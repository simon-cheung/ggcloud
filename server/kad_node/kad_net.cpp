#include "stdafx.h"
#include "o_net_conf.pb.h"
#include "kad_net.h"

namespace oo{

    kad_net::kad_net(void)
    {
    }

    kad_net::~kad_net(void)
    {
    }

    std::string node_id::to_hex_string(const node_id& nid){
        std::stringstream ss;
        ss.precision(8);

        for (int i=0; i < Max_Kad_MBytes; i++)
	    {
            ss << std::hex << nid.data[i];
	    }
        return ss.str();
    }

    node_id node_id::from_hex_string(const std::string& str){
        node_id nid;
        std::stringstream ss(str);
        ss.precision(8);

        for (int i=0; i < Max_Kad_MBytes; i++)
	    {
            ss >> std::hex >> nid.data[i];
	    }
        return nid;
    }

    bool kad_net::load_conf(const std::string& conf){
        conf::local_node_conf config;
        google::protobuf::TextFormat::ParseFromString(conf, &config);
     
        cur_node_info_.Swap(config.mutable_local());
        merge_node_lst(config.others().data(), config.others_size());

        return true;
    }

    void kad_net::save_conf(std::string& conf){
        conf::local_node_conf config;
        config.mutable_local()->CopyFrom(cur_node_info_);
        
        //k_bucket  k_bucket_[Max_Kad_level]
        for(int i = 0; i < Max_Kad_level; i++){
            for(k_bucket::iterator it = k_bucket_[i].begin(); it != k_bucket_[i].end(); it++){
                config.mutable_others()->Add()->CopyFrom(*it);
            }// end for in bucket
        }//end for all k_bucket

        google::protobuf::TextFormat::PrintToString(config, &conf);
    }

    node_id distance(const node_id& n1, const node_id& n2) {
        node_id dis;
        for(int i = 0; i < Max_Kad_MBytes; i++){
            dis[i] = n1[i] ^ n2[i];
        }
        return dis;
    }

    size_t kvalue(node_id dis){        
        unsigned char* pb = (unsigned char*)(dis.data);
        size_t exp = 0;
        int i = Max_Kad_Bytes - 1;
        for(; i >= 0; i--){
            if(pb[i] == 0)
                continue;
            exp = pb[i] >= 0x80 ? 7 :
                pb[i] >= 0x40 ? 6 :
                pb[i] >= 0x20 ? 5 :
                pb[i] >= 0x10 ? 4 :
                pb[i] >= 0x08 ? 3 :
                pb[i] >= 0x04 ? 2 :
                1;
        }        
        exp += i * 8;
        return exp;
    }

    size_t kvalue(const node_id& n1, const node_id& n2){
        node_id dis = distance(n1, n2);
        return kvalue(dis);
    }

    int kad_net::merge_node_lst(const node_info* const* ppni, size_t count){
        for(size_t i = 0; i < count; i++){
            place_node(*(ppni[i]));
        }
        return 0;
    }

    int kad_net::merge_node_lst(std::vector<node_info>& info){
        for(std::vector<node_info>::iterator it = info.begin(); it != info.end(); it++){
            place_node(*it);
        }
        return 0;
    }

    int kad_net::place_node(const node_info& ni){
        size_t kv = kvalue(node_id::from_hex_string(ni.id()), 
                            node_id::from_hex_string(cur_node_info_.id()));
        k_bucket& kb = k_bucket_[kv];
        k_bucket::iterator itk = kb.begin();
        for( ; itk != kb.end(); itk++){
            if(itk->id() == ni.id()) {// there is
                time_t ot = itk->ping_time();
                *itk = ni;
                itk->set_ping_time(ot);
                break;
            }
        }

        // 没有发现重复，加入队尾
        if(itk == kb.end()){
            kb.push_back(ni);
        }
        return 0;
    }

    int kad_net::remove_node(const node_id& dest){
        std::string strdest = node_id::to_hex_string(dest);
        size_t kv = kvalue(dest, node_id::from_hex_string(cur_node_info_.id()));
        k_bucket& kb = k_bucket_[kv];
        k_bucket::iterator itk = kb.begin();
        for( ; itk != kb.end(); itk++){
            if(itk->id() == strdest) {// there is
                kb.erase(itk);
                break;
            }
        }
        return 0;
    }

    int kad_net::find_shortest(const node_id& dest, std::vector<node_info>& out){
        size_t kv = kvalue(dest, node_id::from_hex_string(cur_node_info_.id()));
        k_bucket& kb = k_bucket_[kv];
        if(!kb.empty()) {
            if(kb.size() > Kad_k){
                out.insert(out.begin(), kb.begin(), kb.begin() + Kad_k);
            }else{
                out = kb;
            }
        }else{
            // 空的，查找临近的
            // 从当前位置kv，向上（+1）向下（-1）同时查找,指导找到Kad_k个节点或者遍历了整个网络
            // 
            size_t need = Kad_k;
            for(int i = 1; need > 0; i++){
                size_t ind_p = kv + i;
                size_t ind_n = kv - i;
                if(ind_p >= Max_Kad_level && ind_n >= Max_Kad_level)
                    break; //  遍历了整个网络
                size_t cnt_p = 0;   // 本次循环，向上取得节点数目
                size_t cnt_n = 0;   // 本次循环，向下取的节点数目
                size_t psize = ind_p < Max_Kad_level ? k_bucket_[ind_p].size() : 0;
                size_t nsize = ind_n < Max_Kad_level ? k_bucket_[ind_n].size() : 0;
                size_t cnt = psize + nsize;
                cnt_p = cnt <= need ? psize 
                    : psize < nsize ? psize : need - nsize;
                cnt_n = cnt <= need ? nsize : need - psize;
                if(cnt_p > 0)
                    out.insert(out.end(), k_bucket_[ind_p].begin(), k_bucket_[ind_p].begin() + cnt_p);
                if(cnt_n > 0)
                    out.insert(out.end(), k_bucket_[ind_n].begin(), k_bucket_[ind_n].begin() + cnt_n);
                need -= cnt_p + cnt_n;
             }
        }
        return 0;
    }
}//end namespace