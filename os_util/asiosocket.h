#pragma once

namespace oo{
    class Session;
    class Connection;
    class Joint;
    
    typedef boost::shared_ptr<Session> SessionPtr;
    typedef boost::function<void (SessionPtr, void*, size_t)> SessionPacketProc;
    typedef boost::function<void (SessionPtr, const boost::system::error_code&)> SessionErrorProc;

    typedef boost::function<void (SessionPtr , std::string service)> HandlerConnect;
    typedef boost::function<void (SessionPtr , void* , size_t )> HandlePacket;
    typedef boost::function<void (SessionPtr , const boost::system::error_code&)> HandleError;
    typedef boost::function<void ()> HandlerInterrupt;
    typedef std::list<SessionPtr>    SessionLst;

    class Session
        : public boost::enable_shared_from_this<Session>
    {
    public:
        enum SessEvent{
            SE_Connecting,
            SE_Connected,
            SE_Read,
            SE_ReadOver,
            SE_Write,
            SE_WriteOver,
            SE_Error
        };

        static const char* SessEventName[];

    protected:
        friend class Joint;

        boost::asio::ip::tcp::socket mSocket;
        std::list<const void*> mOutLst;
        std::string mName;
        void*  mData;
        Joint* mpJoint;
        uint32 mHeader;
        uint8* mpBuffer;

        HandlePacket   mHandlerPacket;
        HandleError    mHandlerError;

        bool   mbWork;
    public:
        Session(Joint* pJoint);
        virtual ~Session();
        void close();
        void write_not_free(const void* buf, size_t len);

        void start();
        // no safe
        void* getData() const { return mData; }
        void setData(void* pdata) { mData = pdata; }

        const std::string& getName() const { return mName; }
        void setName(const std::string& name) { mName = name; }

        const std::string& getEventId(SessEvent se) const;

        void setHandler(HandlePacket hp, HandleError he) { mHandlerPacket = hp; mHandlerError = he; }
    protected:
        void handle_write(const boost::system::error_code& e);
        void handle_read(const boost::system::error_code& e, size_t size);

    };

    class Connection
        : public Session
        , public boost::enable_shared_from_this<Connection>
    {
        typedef boost::enable_shared_from_this<Connection> SubShared;
    public:
        Connection(Joint* pJoint);
        bool connect(const char* address, const char* service);

    protected:
        void handle_connect(const boost::system::error_code& e,
            boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    };

    class Joint
    {
        boost::asio::io_service mIOService;
        boost::asio::io_service::work mLooWork;
        SessionLst      mlstSession;
        HandlePacket   mHandlerPacket;
        HandleError    mHandlerError;
    public:
        static Joint& instance();
        Joint();

        void startup();
        void run();
        void runone();
        void interrupt(HandlerInterrupt hi);
        void stop();

        boost::asio::io_service& io_service() { return mIOService; }
        
        void visit(HandlePacket hp, HandleError he) { mHandlerPacket = hp; mHandlerError = he; }

        bool listenAt(const char* address, const char* service, HandlerConnect hConn);
        SessionPtr connect2(const char* address, const char* service);
    public:
        void addSession(SessionPtr pSession)
        {
            mlstSession.push_back(pSession);
        }

        void removeSession(SessionPtr pSession)
        {
            mlstSession.remove(pSession);
        }

        SessionPtr getSession(const std::string& name)
        {
            return SessionPtr();
        }
    public:
        virtual void onConnect(SessionPtr pSession, boost::asio::ip::tcp::acceptor* acc, std::string service, HandlerConnect hCon);

        void onPacket(SessionPtr pSession, void* buf, size_t len)
        {
            if(mHandlerPacket)
                mHandlerPacket(pSession, buf, len);
        }
        void onError(SessionPtr pSession, const boost::system::error_code& e)
        {
            if(mHandlerPacket)
                mHandlerError(pSession, e);
        }
    };
}