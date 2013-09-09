#pragma once
namespace oo{

    class oriole_user : public oriole_object
    {
        Message* mState;
        bool     mbInit;
    public:
        oriole_user(void);
        ~oriole_user(void);

        ZeroCopyOutputStream& serialize(ZeroCopyOutputStream& istream);
        ZeroCopyInputStream& deserialize(ZeroCopyInputStream& ostream);

        const std::string& getId() const;
        int proc_msg(const std::string& from, Message* msg);
    protected:
        void handle_hearttick(Message* msg);
        void handle_enter_room(Message* msg);
        void handle_enter_room_update(Message* msg);
        void handle_leave_room(Message* msg);
        void handle_leave_room_update(Message* msg);
        void handle_sendto_user(Message* msg);
        void handle_sendto_user_update(Message* msg);
        void handle_sendto_room(Message* msg);
        void handle_sendto_room_update(Message* msg);
        
        void handle_create_room(Message* msg);
        void handle_active_room(Message* msg);
        void handle_destory_room(Message* msg);
        void handle_close_room(Message* msg);
        void handle_lst_room(Message* msg);
        void handle_active_room_live_voice(Message* msg);
        void handle_deactive_room_live_voice(Message* msg);

        // result
        void handle_enter_room_result(Message* msg);
        void handle_leave_room_result(Message* msg);
        void handle_sendto_user_result(Message* msg);
        void handle_sendto_room_result(Message* msg);
        
        void handle_create_room_result(Message* msg);
        void handle_active_room_result(Message* msg);
        void handle_destory_room_result(Message* msg);
        void handle_close_room_result(Message* msg);
        void handle_lst_room_result(Message* msg);
        void handle_active_room_live_voice_result(Message* msg);
        void handle_deactive_room_live_voice_result(Message* msg);
    };

}