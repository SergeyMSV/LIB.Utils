///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPortSerial
// 2021-11-02 - 2026-10-15
// C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include <boost/asio.hpp>

namespace utils
{
namespace port
{
namespace serial
{

using tCharSize = boost::asio::serial_port_base::character_size;
using tStopBits = boost::asio::serial_port_base::stop_bits::type;
using tParity = boost::asio::serial_port_base::parity::type;
using tFlowControl = boost::asio::serial_port_base::flow_control::type;

// All operations on the port are serialized through a strand.
// A derived class shall call Close() in its destructor so that OnReceived() is not called on a partially destroyed object.
// io_context shall be stopped (or the port closed) before the object is destroyed.
template<std::size_t DataSizeRecv = 1024>
class tPortSerialAsync
{
	using tStrand = boost::asio::strand<boost::asio::io_context::executor_type>;

	tStrand m_Strand;
	boost::asio::serial_port m_Port;

	std::uint8_t m_Data[DataSizeRecv]{};

	std::queue<std::vector<std::uint8_t>> m_DataSent; // accessed only within m_Strand

	mutable std::mutex m_ErrorCodeMtx;
	boost::system::error_code m_ErrorCode;

	std::shared_ptr<int> m_Alive = std::make_shared<int>(0);

public:
	tPortSerialAsync(boost::asio::io_context& ioc, const std::string& id, std::uint32_t baudRate, tCharSize charSize, tStopBits stopBits, tParity parity, tFlowControl flowControl)
		:m_Strand(boost::asio::make_strand(ioc)), m_Port(m_Strand)
	{
		boost::system::error_code ec;
		m_Port.open(id, ec);
		if (!ec) m_Port.set_option(boost::asio::serial_port_base::baud_rate(baudRate), ec);
		if (!ec) m_Port.set_option(boost::asio::serial_port_base::character_size(charSize), ec);
		if (!ec) m_Port.set_option(boost::asio::serial_port_base::stop_bits(stopBits), ec);
		if (!ec) m_Port.set_option(boost::asio::serial_port_base::parity(parity), ec);
		if (!ec) m_Port.set_option(boost::asio::serial_port_base::flow_control(flowControl), ec);

		if (ec)
		{
			SetError(ec);
			boost::system::error_code ecClose;
			m_Port.close(ecClose);
			return;
		}

		Receive();
	}

	tPortSerialAsync(boost::asio::io_context& ioc, const std::string& id, std::uint32_t baudRate)
		:tPortSerialAsync(ioc, id, baudRate, tCharSize(8), tStopBits::one, tParity::none, tFlowControl::none)
	{
	}

	tPortSerialAsync(const tPortSerialAsync&) = delete;
	tPortSerialAsync& operator=(const tPortSerialAsync&) = delete;

	virtual ~tPortSerialAsync()
	{
		m_Alive.reset();
		boost::system::error_code ec;
		if (m_Port.is_open())
		{
			m_Port.cancel(ec); // Force all pending async ops to complete with operation_aborted
			m_Port.close(ec);
		}
	}

	void Close()
	{
		RunInStrand([this]()
		{
			if (!m_Port.is_open())
				return;
			boost::system::error_code ec;
			m_Port.cancel(ec);
			m_Port.close(ec);
			SetError(boost::asio::error::not_connected);
		});
	}

	bool IsReady() const
	{
		std::lock_guard<std::mutex> Lock(m_ErrorCodeMtx);
		return !m_ErrorCode;
	}

	boost::system::error_code GetError() const
	{
		std::lock_guard<std::mutex> Lock(m_ErrorCodeMtx);
		return m_ErrorCode;
	}

	std::uint32_t GetBaudRate()
	{
		std::uint32_t Value = 0;
		RunInStrand([this, &Value]()
		{
			if (!m_Port.is_open())
				return;
			boost::asio::serial_port_base::baud_rate br{};
			boost::system::error_code ec{};
			m_Port.get_option(br, ec);
			if (!ec)
				Value = br.value();
		});
		return Value;
	}

	bool SetBaudRate(std::uint32_t val)
	{
		if (val == 0)
			return false;
		bool Result = false;
		RunInStrand([this, val, &Result]()
		{
			if (!m_Port.is_open())
				return;
			boost::system::error_code ec;
			m_Port.set_option(boost::asio::serial_port_base::baud_rate(val), ec);
			Result = !ec;
		});
		return Result;
	}

	bool Send(const std::vector<std::uint8_t>& data)
	{
		if (data.empty() || !IsReady())
			return false;

		// [TBD] Something shall limit incoming data for sending here.

		boost::asio::post(m_Strand, [this, data, Alive = std::weak_ptr<int>(m_Alive)]() mutable
		{
			if (Alive.expired())
				return;
			const bool StartSending = m_DataSent.empty();
			m_DataSent.push(std::move(data));
			if (StartSending)
				DoSend();
		});

		return true;
	}

protected:
	virtual void OnReceived(const std::vector<std::uint8_t>& data) = 0;

private:
	void SetError(const boost::system::error_code& ec)
	{
		std::lock_guard<std::mutex> Lock(m_ErrorCodeMtx);
		if (!m_ErrorCode)
			m_ErrorCode = ec;
	}

	// Executes func within the strand and waits for completion.
	// If io_context is not running, func is executed in the caller's thread.
	template<typename TFunc>
	void RunInStrand(TFunc func)
	{
		auto& Ctx = static_cast<boost::asio::io_context&>(m_Strand.context());
		if (m_Strand.running_in_this_thread() || Ctx.stopped())
		{
			func();
			return;
		}
		std::packaged_task<void()> Task(std::move(func));
		std::future<void> Future = Task.get_future();
		boost::asio::post(m_Strand, [&Task]() { Task(); });
		Future.wait();
	}

	void Receive()
	{
		m_Port.async_read_some(boost::asio::buffer(m_Data, sizeof(m_Data)),
			[this, Alive = std::weak_ptr<int>(m_Alive)](boost::system::error_code ec, std::size_t bytes_recvd)
			{
				if (Alive.expired())
					return;

				if (!ec && bytes_recvd > 0)
				{
					const std::vector<std::uint8_t> Data(m_Data, m_Data + bytes_recvd);
					OnReceived(Data);
				}

				if (ec)
				{
					SetError(ec);
					return;
				}

				Receive();
			});
	}

	void DoSend()
	{
		boost::asio::async_write(m_Port, boost::asio::buffer(m_DataSent.front()),
			[this, Alive = std::weak_ptr<int>(m_Alive)](boost::system::error_code ec, std::size_t /*bytes_sent*/)
			{
				if (Alive.expired())
					return;

				if (ec)
				{
					SetError(ec);
					m_DataSent = {};
					return;
				}

				m_DataSent.pop();

				if (!m_DataSent.empty())
					DoSend();
			});
	}
};

class tPortOneWireSync
{
	class tPort : public tPortSerialAsync<64>
	{
		mutable std::mutex m_Mtx;
		std::condition_variable m_Cv;
		std::vector<std::uint8_t> m_DataRcv;

	public:
		tPort(boost::asio::io_context& ioc, const std::string& id, std::uint32_t baudRate, tCharSize charSize, tStopBits stopBits, tParity parity, tFlowControl flowControl);
		~tPort() override;

		bool WaitReceived(std::size_t qty, std::chrono::microseconds timeout);
		std::vector<std::uint8_t> GetReceived(std::size_t qty);
		void ResetReceived();

	protected:
		void OnReceived(const std::vector<std::uint8_t>& data) override;
	};

	struct tGuardBR
	{
		tPort* m_pPort;
		std::uint32_t m_BR = 0;

		tGuardBR() = delete;
		tGuardBR(tPort* port, std::uint32_t baudRate);
		~tGuardBR();
	};

	tPort m_Port;

public:
	enum class tStatus
	{
		Success,
		NoRsp,
		PortDamaged,
		ShortCircuit,
		Presence,
	};

	enum class tSpeed
	{
		Slow, // 57600
		Norm, // 115200
		Fast, // 230400
	};

public:
	tPortOneWireSync() = delete;
	tPortOneWireSync(boost::asio::io_context& ioc, const std::string& id, tSpeed speed = tSpeed::Norm);

	void Close() { m_Port.Close(); }
	bool IsReady() const { return m_Port.IsReady(); }
	boost::system::error_code GetError() const { return m_Port.GetError(); }

	tStatus Reset();
	std::vector<std::uint8_t> Transaction(std::vector<std::uint8_t> tx, std::size_t rxSize);

protected:
	void SendBit(bool tx);
	std::vector<bool> ReceiveBits(std::size_t rxSize);

private:
	std::vector<std::uint8_t> OW_Transaction(const std::vector<std::uint8_t>& tx);

	std::vector<std::uint8_t> GetReceived(std::size_t rxSize);

protected:
	static std::vector<std::uint8_t> ByteToBit(const std::vector<std::uint8_t>& data);
	static std::vector<std::uint8_t> BitToByte(const std::vector<std::uint8_t>& data);
	static std::uint8_t BitToByte(std::uint8_t val) { return val == 0xFF ? 1 : 0; }

	static std::uint32_t ToBaudRate(tSpeed val);
};

}
}
}
