#include "oriole_pch.h"
#include "oriole_object.h"
#include "oriole_user.h"
#include "oriole_room.h"
#include "protocol/o_s_user.pb.h"

namespace oo{

    oriole_user::oriole_user(void) : mbInit(false)
    {
        mState = new oo::proto::user_state;
    }


    oriole_user::~oriole_user(void)
    {
        delete mState;
        mState = NULL;
    }

    ZeroCopyOutputStream& oriole_user::serialize(ZeroCopyOutputStream& ostream){
        if(!mbInit)
            return ostream;

        mState->SerializeToZeroCopyStream(&ostream);
        return ostream;
    }

    ZeroCopyInputStream& oriole_user::deserialize(ZeroCopyInputStream& istream){
        if(mbInit)
            return istream;

        oo::proto::user_state us;
        us.ParseFromZeroCopyStream(&istream);

        mbInit = true;
        return istream;
    }

    const std::string& oriole_user::getId() const{
        oo::proto::user_state* us = (oo::proto::user_state*)mState;
        return us->user();
    }

    int oriole_user::proc_msg(const std::string& from, Message* msg){
        oo::proto::user_state* us = (oo::proto::user_state*)mState;


    }
    using namespace oo::proto;
    void oriole_user::handle_hearttick(Message* msg){
        // do nothing
    }

    void oriole_user::handle_enter_room(Message* msg){
        user_enter_room* umsg = (user_enter_room*)msg;
        std::string mbuf;
        msg->SerializeToString(&mbuf);
        send_msg(getId().c_str(), umsg->room_name(), mbuf.c_str(), mbuf.length());
        // timeout or wait result
    }

    void oriole_user::handle_enter_room_update(Message* msg){
    }

    void oriole_user::handle_leave_room(Message* msg){
    }

    void oriole_user::handle_leave_room_update(Message* msg){
    }

    void oriole_user::handle_sendto_user(Message* msg){
    }

    void oriole_user::handle_sendto_user_update(Message* msg){
    }

    void oriole_user::handle_sendto_room(Message* msg){
    }

    void oriole_user::handle_sendto_room_update(Message* msg){
    }

    void oriole_user::handle_create_room(Message* msg){
    }

    void oriole_user::handle_active_room(Message* msg){
    }

    void oriole_user::handle_destory_room(Message* msg){
    }

    void oriole_user::handle_close_room(Message* msg){
    }

    void oriole_user::handle_lst_room(Message* msg){
    }

    void oriole_user::handle_active_room_live_voice(Message* msg){
    }

    void oriole_user::handle_deactive_room_live_voice(Message* msg){
    }


    // result
    void oriole_user::handle_enter_room_result(Message* msg){
    }

    void oriole_user::handle_leave_room_result(Message* msg){
    }

    void oriole_user::handle_sendto_user_result(Message* msg){
    }

    void oriole_user::handle_sendto_room_result(Message* msg){
    }
        
    void oriole_user::handle_create_room_result(Message* msg){
    }

    void oriole_user::handle_active_room_result(Message* msg){
    }

    void oriole_user::handle_destory_room_result(Message* msg){
    }

    void oriole_user::handle_close_room_result(Message* msg){
    }

    void oriole_user::handle_lst_room_result(Message* msg){
    }

    void oriole_user::handle_active_room_live_voice_result(Message* msg){
    }

    void oriole_user::handle_deactive_room_live_voice_result(Message* msg){
    }

}