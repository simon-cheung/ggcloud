#include "ospch.h"
#include "asiosocket.h"

namespace oo{
    using namespace boost::asio;
    using boost::asio::ip::tcp;

    Session::Session(Joint* pJoint)
        : mSocket(pJoint->io_service())
        , mData(NULL)
        , mpJoint(pJoint)
        , mpBuffer(NULL)
        , mbWork(false)
    {
        std::stringstream ss;
        ss << this << time(0);
        mName = ss.str();
    }

    Session::~Session()
    {
    }

    void Session::start()
    {
        mHeader = 0;
        mpBuffer = NULL;
        // Successfully established connection. Start operation to read the list
        // of stocks. The connection::async_read() function will automatically
        // decode the data that is read from the underlying socket.
        mSocket.async_receive(boost::asio::buffer(&mHeader, sizeof(mHeader)),
            boost::bind(&Session::handle_read, shared_from_this(),boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

        mbWork = true;
    }

    void Session::close()
    {
        STACK_TRACE;
        NAFInfo("session [%x] close, socket : %d", this, mSocket.native());

        mbWork = false;
        mSocket.close();
        mHeader = 0;
        mpBuffer = NULL;
    }

    void Session::write_not_free(const void* buf, size_t len)
    {
        if(!mbWork)
            return;
        void* pbuf = malloc(len + sizeof(len));
        memcpy(pbuf, &len, sizeof(len));
        memcpy((char*)pbuf + sizeof(len), buf, len);
     
        mSocket.async_send(boost::asio::buffer(pbuf, len + sizeof(len)), boost::bind(&Session::handle_write, shared_from_this(), boost::asio::placeholders::error));

        free(pbuf);
    }

    void Session::handle_write(const boost::system::error_code& e)
    {
        if(!mbWork)
            return;

        //free((void*)(mOutLst.front()));
        //mOutLst.pop_front();
        if(e)
        {
            if(mHandlerError)
                mHandlerError(shared_from_this(), e);
            else
                mpJoint->onError(shared_from_this(), e);
            close();
        }
    }

    void Session::handle_read(const boost::system::error_code& e, size_t size)
    {
        if(!mbWork)
            return;

        if(e)
        {
            if(mpBuffer)
                free(mpBuffer);

            if(mHandlerError)
                mHandlerError(shared_from_this(), e);
            else
                mpJoint->onError(shared_from_this(), e);
            close();

            return;
        }

        if(mHeader != 0)    // header data
        {
            mpBuffer = (uint8*)malloc(mHeader);
            mSocket.async_receive(boost::asio::buffer(mpBuffer, mHeader),
                boost::bind(&Session::handle_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            mHeader = 0;
        }
        else                
        {
            // body data
            if(mHandlerPacket)
                mHandlerPacket(shared_from_this(), mpBuffer, size);
            else
                mpJoint->onPacket(shared_from_this(), mpBuffer, size);
            // to worker
            start(); 
        }
    }

    //============================================================================
    /// Handle completion of a connect operation.
    Connection::Connection(Joint* pJoint)
        : Session(pJoint)
    {
    }

    void Connection::handle_connect(const boost::system::error_code& e,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    {
        if (!e)
        {
            start();
        }
        else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
        {
            // Try the next endpoint.
            mSocket.close();
            boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
            mSocket.async_connect(endpoint,
                boost::bind(&Connection::handle_connect, SubShared::shared_from_this(),
                boost::asio::placeholders::error, ++endpoint_iterator));
        }
        else
        {
            // An error occurred. Log it and return. Since we are not starting a new
            // operation the io_service will run out of work to do and the client will
            // exit.
            std::cerr << e.message() << std::endl;
        }
    }

    bool Connection::connect(const char* addr, const char* service)
    {
        STACK_TRACE;
        NAFInfo("connect 2 %s:%s", addr, service);
        // Resolve the host name into an IP address.
        boost::asio::ip::tcp::resolver resolver(mSocket.io_service());
        boost::asio::ip::tcp::resolver::query query(addr, service);
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        //mSocket.async_connect(endpoint,
        //    boost::bind(&Connection::handle_connect, _MyShared::shared_from_this(),
        //    boost::asio::placeholders::error, ++endpoint_iterator));

        // wait
        try{
        for(; endpoint_iterator != tcp::resolver::iterator(); ++endpoint_iterator)
        {
            mSocket.connect(*endpoint_iterator);
            break;
        }
        
        NAFInfo("connect 2 %s:%s suc", addr, service);
        NAFInfo("session start, open read");
        start();
        }
        catch(boost::system::error_code& e)
        {
            std::string stre = e.message();
            NAFInfo("connect 2 %s:%s error: %s", addr, service, stre);
            mSocket.close();
            std::cerr << stre << std::endl;
            return false;
        }
        catch(std::exception& e)
        {
            NAFInfo("connect 2 %s:%s error: %s", addr, service, e.what());
            mSocket.close();
            std::cerr << e.what() << std::endl;
            return false;
        }

        return true;
    }

    //=======================================================================================
    Joint& Joint::instance()
    {
        static Joint __Inst__;
        return __Inst__;
    }

    Joint::Joint()
        : mLooWork(mIOService)
    {
    }

    void Joint::startup()
    {
    }

    void Joint::runone()
    {
        mIOService.run_one();
    }

    void Joint::run()
    {
        mIOService.run();
    }

    void JointInterrupt(HandlerInterrupt hi, const boost::system::error_code& error, std::size_t bytes_transferred)
    {

    }


    void Joint::interrupt(HandlerInterrupt hi)
    {
        mIOService.post(hi);
    }

    void Joint::stop()
    {
        STACK_TRACE;
        mIOService.stop();
    }

    SessionPtr Joint::connect2(const char* address, const char* service)
    {
        STACK_TRACE;
        NAFInfo("start conenct 2 %s:%s", address, service);
        Connection* con = new Connection(this);
        SessionPtr pNew(con);
        if( con->connect(address, service) )
            return pNew;
        return SessionPtr();
    }

    void Joint::onConnect(SessionPtr pSession, tcp::acceptor* acc, std::string service, HandlerConnect hCon)
    {
        STACK_TRACE;
        NAFInfo("new conenct from servie %s", service.c_str());
        if(hCon)
        {
            NAFInfo("session start, open read");

            pSession->start();
            hCon(pSession, service);
        }

        SessionPtr pNew(new Session(this));
        acc->async_accept(pNew->mSocket, boost::bind(&Joint::onConnect, this, pNew, acc, service, hCon));
    }

    bool Joint::listenAt(const char* address, const char* service, HandlerConnect hConn)
    {
        STACK_TRACE;
        NAFInfo("listen @ %s:%s", address, service);
        // Resolve the host name into an IP address.
        boost::asio::ip::tcp::resolver resolver(mIOService);
        boost::asio::ip::tcp::resolver::query query(address, service);
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        //mSocket.async_connect(endpoint,
        //    boost::bind(&Connection::handle_connect, _MyShared::shared_from_this(),
        //    boost::asio::placeholders::error, ++endpoint_iterator));

        // wait
        tcp::acceptor* pacc = NULL;
        try{
        SessionPtr pNew(new Session(this));
        for(; endpoint_iterator != tcp::resolver::iterator(); ++endpoint_iterator)
        {
            pacc = new tcp::acceptor(mIOService, *endpoint_iterator);
            break;
        }
            NAFInfo("listen @ %s:%s, suc", address, service);
            pacc->async_accept(pNew->mSocket, boost::bind(&Joint::onConnect, this, pNew, pacc, service, hConn));
        }
        catch(boost::system::error_code& e)
        {
            std::string stre = e.message();
            if(pacc)
                delete pacc;
            NAFInfo("listen @ %s:%s, error", address, service, stre.c_str());
            std::cerr << stre << std::endl;
            return false;
        }
        catch(std::exception& e)
        {
            if(pacc)
                delete pacc;
            NAFInfo("listen @ %s:%s, error", address, service, e.what());
            std::cerr << e.what() << std::endl;
            return false;
        }

        return true;
    }
}