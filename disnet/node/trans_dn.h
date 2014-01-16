#ifndef __trans_dn_h__
#define __trans_dn_h__

namespace dn{
    struct op{
        void* handler_;
    };

    class trans{
        std::list<op*> step_;
	private:// noncopy
		trans(const trans&);
		trans& operator=(const trans&);
    public:
		trans();
		virtual ~trans();

		virtual void run() = 0;

        void step(std::function<void ()> func);

        template<class Handler>
        void async_step(Handler hd){
            op* op_ = new op;
            op_->handler_ = (void*)(new Handler(hd));
            step_.push_back(op_);
        }

        void post_step(){
            op* op_ = step_.front();
            std::function<void ()>* cs = static_cast<std::function<void ()>* >(op_->handler_);
            (*cs)();
            step_.pop_front();
        }

        template<class arg1>
        void post_step(arg1 a1){
            op* op_ = step_.front();
            std::function<void (arg1)>* cs = static_cast<std::function<void (arg1)>* >(op_->handler_);
            (*cs)(a1);
            step_.pop_front();
        }

        template<class arg1,class arg2>
        void post_step(arg1 a1, arg2 a2){
            op* op_ = step_.front();
            std::function<void (arg1,arg2)>* cs = static_cast<std::function<void (arg1, arg2)>* >(op_->handler_);
            (*cs)(a1, a2);
            step_.pop_front();
        }

        template<class arg1,class arg2, class arg3>
        void post_step(arg1 a1, arg2 a2, arg3 a3){
            op* op_ = step_.front();
            std::function<void (arg1, arg2, arg3)>* cs = static_cast<std::function<void (arg1, arg2, arg3)>* >(op_->handler_);
            (*cs)(a1);
            step_.pop_front();
        }
    };
}

#endif