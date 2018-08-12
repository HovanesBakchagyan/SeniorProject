/**
 * \file tcpdatatransport.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief TCP transport for reader/card commands.
 */

#ifndef LOGICALACCESS_TCPDATATRANSPORT_HPP
#define LOGICALACCESS_TCPDATATRANSPORT_HPP

#include "logicalaccess/readerproviders/datatransport.hpp"
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace logicalaccess
{
#define	TRANSPORT_TCP			"TCP"

    /**
     * \brief An TCP data transport class.
     */
    class LIBLOGICALACCESS_API TcpDataTransport : public DataTransport
    {
    public:

        /**
         * \brief Constructor.
         */
        TcpDataTransport();

        /**
         * \brief Destructor.
         */
        virtual ~TcpDataTransport();

        /**
         * \brief Get the transport type of this instance.
         * \return The transport type.
         */
        virtual std::string getTransportType() const { return TRANSPORT_TCP; };

		/**
         * \brief Connect to the transport layer.
         * \return True on success, false otherwise.
         */
        virtual bool connect();

        /**
         * \brief Connect to the transport layer.
		 * \param timeout Time after the connect task will be canceled.
         * \return True on success, false otherwise.
         */
        virtual bool connect(long int timeout);

        /**
         * \param Disconnect from the transport layer.
         */
        virtual void disconnect();

        /**
         * \brief Get if connected to the transport layer.
         * \return True if connected, false otherwise.
         */
        virtual bool isConnected();

        /**
         * \brief Get the data transport endpoint name.
         * \return The data transport endpoint name.
         */
        virtual std::string getName() const;

        /**
         * \brief Serialize the current object to XML.
         * \param parentNode The parent node.
         */
        virtual void serialize(boost::property_tree::ptree& parentNode);

        /**
         * \brief UnSerialize a XML node to the current object.
         * \param node The XML node.
         */
        virtual void unSerialize(boost::property_tree::ptree& node);

        /**
         * \brief Get the default Xml Node name for this object.
         * \return The Xml node name.
         */
        virtual std::string getDefaultXmlNodeName() const;

        /**
         * \brief Get the ip address.
         * \return The ip address.
         */
        std::string getIpAddress() const;

        /**
         * \brief Set the ip address.
         * \param ipAddress The ip address.
         */
        void setIpAddress(std::string ipAddress);

        /**
         * \brief Get the port.
         * \return The port.
         */
        int getPort() const;

        /**
         * \brief Set the port.
         * \param port The port.
         */
        void setPort(int port);

        /**
         * \brief Send data packet
         * \param data The packet.
         */
        virtual void send(const std::vector<unsigned char>& data);

        /**
         * \brief Receive packet
         * \param timeout Time waiting for data.
         * \return The data receive.
         */
        virtual std::vector<unsigned char> receive(long int timeout);

		/**
         * \brief Connect complete
		 * \param error Read error
         */
		void connect_complete(const boost::system::error_code& error);

		/**
         * \brief Read complete
		 * \param error Read error
		 * \param bytes_transferred Byte transfered
         */
		void read_complete(const boost::system::error_code& error, size_t bytes_transferred);

		/**
         * \brief Read timeout
		 * \param error Read timeout or canceled
         */
	    void time_out(const boost::system::error_code& error);

    protected:

        /**
         * \brief Provides core I/O functionality
         */
        boost::asio::io_service d_ios;

        /**
         * \brief TCP Socket
         */
		boost::asio::ip::tcp::socket d_socket;

		/**
         * \brief Read Deadline timer
         */
		boost::asio::deadline_timer d_timer;

		/**
         * \brief Read error
         */
		bool d_read_error;

		/**
         * \brief Byte Readed
         */
		size_t d_bytes_transferred;

        /**
         * \brief The ip address
         */
        std::string d_ipAddress;

        /**
         * \brief The listening port.
         */
        int d_port;
    };
}

#endif /* LOGICALACCESS_TCPDATATRANSPORT_HPP */