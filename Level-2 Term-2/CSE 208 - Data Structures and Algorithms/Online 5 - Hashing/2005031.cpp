#include<bits/stdc++.h>
using namespace std;

int max(int a, int b){
    if(a>b) return a;
    else return b;
}

// Node struct
struct Node{
    string key;
    int value;
    bool deleted=true;

    Node(string key="", int value=0, bool deleted=true){
        this->key=key;
        this->value=value;
        this->deleted=deleted;
    }

    Node(pair<string,int>k_v_pair, bool deleted=false){
        this->key=k_v_pair.first;
        this->value=k_v_pair.second;
        this->deleted=deleted;
    }
};


// Hash Table Class
class  HashTable{
    int number_of_delete=0;
    int number_of_insert=0;
    int chain_length_limit;
    int number_of_elements;
    int min_length;
    int number_of_collisions;
    int number_of_probes;
    int number_of_find;
    int hash_table_size;
    int hash_number;
    string collision_resolution;
    vector<list<Node>> chain_hash_table;
    vector<Node> hash_table;

    int count_max_chain_length(){
        int maxx=0;
        for(auto x:chain_hash_table)
            maxx=max(x.size(),maxx);
        cout<<maxx;
    }

    double get_load_factor(){
        return 1.0*number_of_elements/hash_table_size;
    }

    void rehash(int flag){
        if(flag){
            HashTable newtable(2*hash_table_size,"sc",1,chain_length_limit);
            for(auto x:chain_hash_table)
                for(auto y:x)
                    newtable.insert_key(make_pair(y.key,y.value));
            this->chain_hash_table = newtable.chain_hash_table;
            hash_table_size=newtable.hash_table_size;
        }
        else{
            if(hash_table_size/2<=min_length)
                return;
            HashTable newtable(hash_table_size/2,"sc",1,chain_length_limit);
            for(auto x:chain_hash_table)
                for(auto y:x)
                    newtable.insert_key(make_pair(y.key,y.value));
            this->chain_hash_table = newtable.chain_hash_table;
            hash_table_size=newtable.hash_table_size;
        }
        cout<<"load factor: "<<get_load_factor()<<endl;
        cout<<"average probe: "<<1.0*number_of_collisions/number_of_insert;
        cout<<"maximum chain length: "<<count_max_chain_length()+1;
    }


    bool is_prime(int number){
        bool prime=true;
        for(int j=2;j<=sqrt(number)&&prime;j++)
            if(!(number%j)){
                prime=false;
                break;
            }
        return prime;
    }


    int get_nearest_not_smaller_prime_number(int number){
        for(int i=number;;i++)
            if(is_prime(i))
                return i;
        return 0;
    }



    int hash1(string key){
        int considered_string_length=key.size();
        if(considered_string_length>10)
            considered_string_length=10;
        int hashed_index=0;
        for(int i=0;i<considered_string_length;i++){
            hash<string> hs;
            hashed_index=(hs(key)%hash_table_size);

            // hashed_index=(hashed_index*17+(int)(key[i]))%hash_table_size;
            // hashed_index=(hashed_index*317+(int)(key[i])*(int)(key[i]))%hash_table_size;
        }
        if(hashed_index<0)
            hashed_index+=hash_table_size;
        return hashed_index;
    }


    int hash2(string key){
        int considered_string_length=key.size();
        if(considered_string_length>10)
            considered_string_length=10;
        int hashed_index=0;
        for(int i=0;i<considered_string_length;i++)
            hashed_index=hash_table_size-((hashed_index*1237+int(key[i])*int(key[i]))*37)%hash_table_size-1;
        if(hashed_index<0)
            hashed_index+=hash_table_size;
        return hashed_index;
    }


    int adjhash(string key){
        int considered_string_length=key.size();
        if(considered_string_length>10)
            considered_string_length=10;
        int hashed_index=0;
        int prime=27;
        for(int i=0;i<considered_string_length;i++)
            hashed_index=(hashed_index+int(key[i]))%prime;
        if(hashed_index<0)
            hashed_index+=prime;
        hashed_index=prime-hashed_index;
        return hashed_index;
    }


    void chain_insert(pair<string,int> k_v_pair){
        number_of_insert++;
        int index;
        if(hash_number==1)
            index=hash1(k_v_pair.first);
        else
            index=hash2(k_v_pair.first);
        for(auto it=chain_hash_table[index].begin();it!=chain_hash_table[index].end();it++)
            if(it->key==k_v_pair.first){
                if(it->deleted)
                    number_of_elements++;
                it->value=k_v_pair.second;
                it->deleted=false;
                return;
            }
            else
                number_of_collisions++;
        number_of_elements++;
        Node newnode(k_v_pair);
        chain_hash_table[index].push_front(newnode);
        if(number_of_insert%100==0)
            if(count_max_chain_length()>chain_length_limit)
                rehash(1);
    }


    void double_hash_insert(pair<string,int> k_v_pair){
        if(number_of_elements==min_length)
            return;
        int index;
        if(hash_number==1)
            index=hash1(k_v_pair.first);
        else
            index=hash2(k_v_pair.first);
        int newhash=adjhash(k_v_pair.first);
        for(int i=1;;i++){
            if(hash_table[index].deleted){
                hash_table[index]=Node(k_v_pair);
                number_of_elements++;
                return;
            }
            else if(hash_table[index].key==k_v_pair.first){
                if(hash_table[index].deleted==true)
                    number_of_elements++;
                hash_table[index].value=k_v_pair.second;
                hash_table[index].deleted=false;
                return;
            }
            else
                number_of_collisions++;
            index=(index+newhash*i)%hash_table_size;
        }
    }


    void custom_probe_insert(pair<string,int> k_v_pair){
        if(number_of_elements==min_length)
            return;
        int c1=1, c2=1, power=1;
        int newhash=adjhash(k_v_pair.first);
        int index;
        if(hash_number==1)
            index=hash1(k_v_pair.first);
        else
            index=hash2(k_v_pair.first);
        for(int i=1;;i++){
            if(hash_table[index].deleted){
                hash_table[index]=Node(k_v_pair);
                number_of_elements++;
                return;
            }
            else if(hash_table[index].key==k_v_pair.first){
                if(hash_table[index].deleted==true)
                    number_of_elements++;
                hash_table[index].value=k_v_pair.second;
                hash_table[index].deleted=false;
                return;
            }
            else
                number_of_collisions++;
            index=(index+c1*i*newhash+c2*i*i)%hash_table_size;
        }
    }

    
    void chain_delete(string key){
        number_of_delete++;
        int index;
        if(hash_number==1)
            index=hash1(key);
        else
            index=hash2(key);
        for(auto it=chain_hash_table[index].begin();it!=chain_hash_table[index].end();it++)
            if(it->key==key){
                chain_hash_table[index].erase(it);
                number_of_elements--;
            }
        if(number_of_delete%100==0)
            if(count_max_chain_length()<chain_length_limit*0.8)
                rehash(0);
    }


    void double_hash_delete(string key){
        int index;
        if(hash_number==1)
            index=hash1(key);
        else
            index=hash2(key);
        int newhash=adjhash(key);
        for(int i=1;;i++){
            if(hash_table[index].deleted)
                return;
            else if(hash_table[index].key==key){
                hash_table[index].deleted=true;
                number_of_elements--;
                return;
            }
            index=(index+newhash*i)%hash_table_size;
        }
    }


    void custom_probe_delete(string key){
        int c1=1, c2=1, power=1;
        int newhash=adjhash(key);
        int index;
        if(hash_number==1)
            index=hash1(key);
        else
            index=hash2(key);
        for(int i=1;;i++){
             if(hash_table[index].deleted)
                return;
            else if(hash_table[index].key==key){
                hash_table[index].deleted=true;
                number_of_elements--;
                return;
            }
            index=(index+c1*i*newhash+c2*i*i)%hash_table_size;
        }
    }


    bool chain_find(string key){
        int index;
        if(hash_number==1)
            index=hash1(key);
        else
            index=hash2(key);
        for(auto it=chain_hash_table[index].begin();it!=chain_hash_table[index].end();it++){
            number_of_probes++;
            if(it->key==key)
                return true;
        }
        return false;
    }


    bool double_hash_find(string key){
        int index;
        if(hash_number==1)
            index=hash1(key);
        else
            index=hash2(key);
        int newhash=adjhash(key);
        int startindex=index;
        for(int i=1;;i++){
            number_of_probes++;
            if(hash_table[index].key==key&&hash_table[index].deleted)
                return false;
            else if(hash_table[index].key==key)
                return true;
            else if(i!=1&&index==startindex)
                return false;
            index=(index+newhash*i)%hash_table_size;
        }
        return false;
    }


    bool custom_probe_find(string key){
        int c1=1, c2=1, power=1;
        int newhash=adjhash(key);
        int index;
        if(hash_number==1)
            index=hash1(key);
        else
            index=hash2(key);
            int startindex=index;
        for(int i=1;;i++){
            number_of_probes++;
            if(hash_table[index].deleted)
                return false;
            else if(hash_table[index].key==key)
                return true;
            else if(i!=1&&index==startindex)
                return false;
            index=(index+c1*i*newhash+c2*i*i)%hash_table_size;
        }
        return false;
    }
    

public:
    HashTable(int hash_table_size, string collision_resolution, int hash_number, int c=-1){
        chain_length_limit=c;
        number_of_elements=0;
        min_length=hash_table_size;
        number_of_collisions=0;
        number_of_find=0;
        number_of_probes=0;
        this->collision_resolution=collision_resolution;
        this->hash_number=hash_number;
        this->hash_table_size=get_nearest_not_smaller_prime_number(hash_table_size);
        if(collision_resolution=="sc")
            chain_hash_table.resize(this->hash_table_size);
        else
            hash_table.resize(this->hash_table_size);
    }


    void insert_key(pair<string,int> k_v_pair){
        if(collision_resolution=="sc")
            chain_insert(k_v_pair);
        else if(collision_resolution=="dh")
            double_hash_insert(k_v_pair);
        else if(collision_resolution=="cp")
            custom_probe_insert(k_v_pair);
    }
    void delete_key(string key){
        if(collision_resolution=="sc")
            chain_delete(key);
        else if(collision_resolution=="dh")
            double_hash_delete(key);
        else if(collision_resolution=="cp")
            custom_probe_delete(key);
    }


    bool find_key(string key){
        bool success=false;
        if(collision_resolution=="sc")
            success=chain_find(key);
        else if(collision_resolution=="dh")
            success=double_hash_find(key);
        else if(collision_resolution=="cp")
            success=custom_probe_find(key);
        number_of_find++;
        return success;
    }

    friend void print_report(HashTable&,HashTable&,HashTable&,HashTable&,HashTable&,HashTable&);
};


void print_report(HashTable &ht1, HashTable &ht2, HashTable &ht3, HashTable &ht4, HashTable &ht5, HashTable &ht6){
    int col1=18;
    int col2=28;
    int col35=16;
    int col46=15;
    int col3456=col35+col46;
    int coltot=col1+col2+col3456*2+7;
    cout<<string(coltot,'=')<<endl;
    cout<<'|';
    cout<<setw(col1)<<"Hash Table Size";
    cout<<'|';
    cout<<setw(col2)<<"Collision Resolution Method";
    cout<<'|';
    cout<<setw(col3456+1)<<"Hash1";
    cout<<'|';
    cout<<setw(col3456+1)<<"Hash2";
    cout<<'|';
    cout<<endl;
    for(int i=0;i<coltot;i++)
        cout<<'_';
    cout<<endl;
    cout<<'|';
    cout<<setw(col1)<<" ";
    cout<<'|';
    cout<<setw(col2)<<" ";
    cout<<'|';
    cout<<setw(col35)<<"# of Collitions";
    cout<<'|';
    cout<<setw(col46)<<"Average Probes";
    cout<<'|';
    cout<<setw(col35)<<"# of Collitions";
    cout<<'|';
    cout<<setw(col46)<<"Average Probes";
    cout<<'|';
    cout<<endl;
    cout<<string(coltot,'=');
    cout<<endl;
    cout<<'|';
    cout<<setw(col1)<<ht1.min_length;
    cout<<'|';
    cout<<setw(col2)<<"Chaining";
    cout<<'|';
    cout<<setw(col35)<<ht1.number_of_collisions;
    cout<<'|';
    cout<<setw(col46)<<(double)ht1.number_of_probes/ht1.number_of_find;
    cout<<'|';
    cout<<setw(col35)<<ht2.number_of_collisions;
    cout<<'|';
    cout<<setw(col46)<<(double)ht2.number_of_probes/ht2.number_of_find;
    cout<<'|';
    cout<<endl;
    cout<<string(coltot,'_');
    cout<<endl;
    cout<<'|';
    cout<<setw(col1)<<" ";
    cout<<'|';
    cout<<setw(col2)<<"Double Hashing";
    cout<<'|';
    cout<<setw(col35)<<ht3.number_of_collisions;
    cout<<'|';
    cout<<setw(col46)<<(double)ht3.number_of_probes/ht3.number_of_find;
    cout<<'|';
    cout<<setw(col35)<<ht4.number_of_collisions;
    cout<<'|';
    cout<<setw(col46)<<(double)ht4.number_of_probes/ht4.number_of_find;
    cout<<'|';
    cout<<endl;
    cout<<string(coltot,'_');
    cout<<endl;
    cout<<'|';
    cout<<setw(col1)<<" ";
    cout<<'|';
    cout<<setw(col2)<<"Custom Probing";
    cout<<'|';
    cout<<setw(col35)<<ht5.number_of_collisions;
    cout<<'|';
    cout<<setw(col46)<<(double)ht5.number_of_probes/ht5.number_of_find;
    cout<<'|';
    cout<<setw(col35)<<ht6.number_of_collisions;
    cout<<'|';
    cout<<setw(col46)<<(double)ht6.number_of_probes/ht6.number_of_find;
    cout<<'|';
    cout<<endl;
    cout<<string(coltot,'=');
    cout<<endl<<endl;
}


struct Random_Word_Generator{
    mt19937 *generator;
    Random_Word_Generator(int seed){
        generator=new mt19937(seed);
    }
    ~Random_Word_Generator(){
        delete generator;
    }
    int get_random_integer(int from, int to){
        uniform_int_distribution<int> distribution(from,to);
        return distribution(*generator);
    }
    string random_word_generator(){
        int length=get_random_integer(5,10);
        string s="";
        for(int i=0;i<length;i++){
            char letter=static_cast<char>(get_random_integer(97,122));
            s+=letter;
        }
        return s;
    }
    vector<pair<string,int>> generate_unique_words_with_index_as_value(int num){
        vector<pair<string,int>> words(num);
        HashTable ht(20000,"sc",1);
        for(int i=0;i<num;i++){
            string word=random_word_generator();
            if(ht.find_key(word))
                i--;
            else{
                ht.insert_key(make_pair(word,i+1));
                words[i]=make_pair(word,i+1);
            }
        }
        return words;
    }
    vector<pair<string,int>> select_random_word_pair(vector<pair<string,int>>words,int num){
        vector<pair<string,int>> selected_words(num);
        HashTable ht(10000,"sc",1);
        for(int i=0;i<num;i++){
            int index=get_random_integer(0,5000-1);
            if(ht.find_key(to_string(index)))
                i--;
            else{
                ht.insert_key(make_pair(to_string(index),0));
                selected_words[i]=words[index];
            }
        }
        return selected_words;
    }
};


int main(){
    // freopen("out.txt","w",stdout);
    int number_of_words=10000;
    int c=-1;
    cout<<"input the number c: ";
    cin>>c;
    int number_of_selected_words=1000;
    Random_Word_Generator rwg(31);
    vector<pair<string,int>> words=rwg.generate_unique_words_with_index_as_value(number_of_words);
    vector<pair<string,int>> selected_words=rwg.select_random_word_pair(words,number_of_selected_words);
    int sizes[]={5000,10000,20000};
    for(int hash_table_size:sizes){
        HashTable htsc5kh1(hash_table_size,"sc",1,c);
        for(int i=0;i<number_of_words;i++)
            htsc5kh1.insert_key(words[i]);
        for(int i=0;i<number_of_words/10;i++)
            htsc5kh1.delete_key(words[i].first);
    }
    return 0;
}