#pragma once

namespace oo{
    const int Kad_k = 8; // bitTorrent的参考值, 保证查询过程是收敛的话，必须小于20
    const int Max_Kad_Bytes = 20;
    const int Max_Kad_MBytes = 20 / sizeof(unsigned long);
    const int Max_Kad_level = Max_Kad_Bytes * 8;

    struct node_id{
        unsigned long data[Max_Kad_MBytes];   

        node_id();

        unsigned long& operator[](int ind) ;
        unsigned long operator[](int ind) const;     
        bool operator==(const node_id& id) const;
        bool operator<(const node_id& id) const;

        static std::string to_hex_string(const node_id& nid);
        static node_id from_hex_string(const std::string& str);

    };

    using oo::proto::node_info;
    typedef std::vector<node_info> k_bucket;
    typedef Hashmap<size_t, node_info> node_map;

    class kad_net{
        k_bucket  k_bucket_[Max_Kad_level]; // kad data
        //node_map  node_map_;                // all node info        
        node_info cur_node_info_;           // current node info
    public:
        kad_net(void);
        ~kad_net(void);

        bool load_conf(const std::string& conf);
        void save_conf(std::string& conf);

        int store(const node_id& key, const std::string& value);
        int find_value(const node_id& key);

        int merge_node_lst(const node_info* const* ppni, size_t count);
        int merge_node_lst(std::vector<node_info>& info);
        int place_node(const node_info& ni);
        int remove_node(const node_id& dest);
        int find_adjacency(const node_id& dest, std::vector<node_info>& out);
        int find_shortest(const node_id& dest, node_inf& out);
        
        const node_info& get_local_node_info() const { return cur_node_info_;}
    };


    inline node_id::node_id()         
    {memset(data, 0, sizeof(data));}

    inline unsigned long& node_id::operator[](int ind){
        return data[ind]; 
    }

    inline unsigned long node_id::operator[](int ind) const { 
        return data[ind]; 
    }

    inline bool node_id::operator==(const node_id& id) const{
        return memcmp(data, id.data, sizeof(node_id)) == 0;
    }

    inline bool node_id::operator<(const node_id& id) const{
        for(int  i = 0; i < Max_Kad_MBytes; i++){
            if(data[i] < id[i])
                return true;
        }
        return false;
    }
}