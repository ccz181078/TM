#include<bits/stdc++.h>
std::mutex mtx;
std::ofstream log_stream("log.txt", std::ios::app);
struct MERR{
	void flush(){
		log_stream.flush();
	}
}merr;
template<class T>
MERR &operator<<(MERR &m,T &x){
	//merr<<x;
	log_stream<<x;
	return m;
}
template<class T>
MERR &operator<<(MERR &m,const T &x){
	//merr<<x;
	log_stream<<x;
	return m;
}
template<class T>
std::ostream &operator<<(std::ostream &os,const std::vector<T> &ls){
	os<<'[';
	bool flag=0;
	for(const T &x:ls){
		if(flag)os<<',';
		os<<x;
		flag=1;
	}
	os<<']';
	return os;
}


template<class T1,class T2>
std::ostream &operator<<(std::ostream &os,const std::pair<T1,T2> &w){
	return os<<'('<<w.first<<','<<w.second<<')';
}

template<class T1,class T2>
std::ostream &operator<<(std::ostream &os,const std::tuple<T1,T2> &w){
	return os<<'('<<std::get<0>(w)<<','<<std::get<1>(w)<<')';
}

template<class T1,class T2,class T3>
std::ostream &operator<<(std::ostream &os,const std::tuple<T1,T2,T3> &w){
	return os<<'('<<std::get<0>(w)<<','<<std::get<1>(w)<<','<<std::get<2>(w)<<')';
}

template<class T1,class T2,class T3,class T4>
std::ostream &operator<<(std::ostream &os,const std::tuple<T1,T2,T3,T4> &w){
	return os<<'('<<std::get<0>(w)<<','<<std::get<1>(w)<<','<<std::get<2>(w)<<','<<std::get<3>(w)<<')';
}

template<class T>
std::string to_str(const T &t){
	std::stringstream ss;
	ss<<t;
	return ss.str();
}
bool dbg=0,to_coq=0,verbose=1;

template<class T>
struct List{
	T v;
	std::shared_ptr<List<T>> prev;
	List(const T &v,std::shared_ptr<List<T>> prev):v(v),prev(prev){}
};

template<class T>
using pList=std::shared_ptr<List<T>>;

template<class T>
pList<T> Cons(const T &x,pList<T> ls){
	return std::make_shared<List<T>>(x,ls);
}

template<class T>
int64_t length(pList<T> ls){
	int64_t s=0;
	while(ls)ls=ls->prev,++s;
	return s;
}

template<class T>
pList<T> rev(pList<T> ls,pList<T> res=nullptr){
	if(ls==nullptr)return res;
	return rev(ls->prev,Cons(rev(ls->v),res));
}
template<class T>
std::ostream &operator<<(std::ostream &os,pList<T> ls){
	while(ls){
		os<<ls->v<<" ";
		ls=ls->prev;
	}
	return os;
}
template<class T>
bool lt(pList<T> a,pList<T> b){
	while(a&&b){
		if(a->v<b->v)return 1;
		if(b->v<a->v)return 0;
		a=a->prev;
		b=b->prev;
	}
	return !a&&b;
}
template<class T>
bool eq(pList<T> a,pList<T> b){
	while(a&&b){
		if(!(a->v==b->v))return 0;
		a=a->prev;
		b=b->prev;
	}
	return !a&&!b;
}
template<class T>
bool eq3(const T &a,const T &b,const T &c){
	return a==b&&a==c;
}
template<class T>
bool eq4(const T &a,const T &b,const T &c,const T &d){
	return a==b&&a==c&&a==d;
}

#define BB6

#ifdef BB64
constexpr int64_t N_STATE=60;
constexpr int64_t N_CHAR=2;
constexpr int64_t log2_N_STATE=6;
constexpr int64_t log2_N_CHAR=1;
typedef bool chr_t;
#endif

#ifdef BB10
constexpr int64_t N_STATE=10;
constexpr int64_t N_CHAR=2;
constexpr int64_t log2_N_STATE=4;
constexpr int64_t log2_N_CHAR=1;
typedef bool chr_t;
#endif

#ifdef BB7
constexpr int64_t N_STATE=7;
constexpr int64_t N_CHAR=2;
constexpr int64_t log2_N_STATE=3;
constexpr int64_t log2_N_CHAR=1;
typedef bool chr_t;
#endif

#ifdef BB6
constexpr int64_t N_STATE=6;
constexpr int64_t N_CHAR=2;
constexpr int64_t log2_N_STATE=3;
constexpr int64_t log2_N_CHAR=1;
typedef bool chr_t;
#endif

#ifdef BB25
constexpr int64_t N_STATE=2;
constexpr int64_t N_CHAR=5;
constexpr int64_t log2_N_STATE=1;
constexpr int64_t log2_N_CHAR=3;
typedef uint8_t chr_t;
#endif

#ifdef BB33
constexpr int64_t N_STATE=3;
constexpr int64_t N_CHAR=3;
constexpr int64_t log2_N_STATE=2;
constexpr int64_t log2_N_CHAR=2;
typedef uint8_t chr_t;
#endif

#ifdef BB43
constexpr int64_t N_STATE=4;
constexpr int64_t N_CHAR=3;
constexpr int64_t log2_N_STATE=2;
constexpr int64_t log2_N_CHAR=2;
typedef uint8_t chr_t;
#endif

constexpr int64_t tm_fmt=2;
struct Trans{
	int64_t nxt,out,dir;
	Trans():nxt(-1),out(1),dir(1){}
	Trans(int64_t s,int64_t o,int64_t d):nxt(s),out(o),dir(d){}
	bool operator==(const Trans &tr)const{
		return nxt==tr.nxt && out==tr.out && dir==tr.dir;
	}
	friend std::ostream &operator<<(std::ostream &os,const Trans &tr){
		os<<("_1"[tr.out])<<' '<<(tr.dir==1?'r':'l')<<' ';
		if(tr.nxt==-1)os<<"halt";
		else os<<tr.nxt;
		return os;
	}
};

struct TM{
	Trans trans[N_STATE][N_CHAR];
	int64_t n_state=1,n_char=1;
	TM(){}
	bool operator==(const TM &tm)const{
		for(int64_t i=0;i<N_STATE;++i)
		for(int64_t j=0;j<N_CHAR;++j){
			if(!(trans[i][j]==tm.trans[i][j]))return 0;
		}
		return 1;
	}
	TM rev()const{
		TM tm=*this;
		for(int64_t s=0;s<N_STATE;++s){
			for(int64_t i=0;i<N_CHAR;++i){
				tm.trans[s][i].dir*=-1;
			}
		}
		return tm;
	}
	friend std::ostream &operator<<(std::ostream &os,const TM &tm){
		int64_t n_state=1,n_char=1;
		for(int64_t s=0;s<N_STATE;++s){
			for(int64_t i=0;i<N_CHAR;++i){
				if(tm.trans[s][i].nxt!=-1){
					n_state=std::max(n_state,s+1);
					n_state=std::max(n_state,tm.trans[s][i].nxt+1);
					n_char=std::max(n_char,i+1);
					n_char=std::max(n_char,tm.trans[s][i].out+1);
				}
			}
		}
		for(int64_t s=0;s<n_state;++s){
			if(s)os<<'_';
			for(int64_t i=0;i<n_char;++i){
				const Trans &tr=tm.trans[s][i];
				if(tr.nxt==-1)os<<"---";
				else os<<char('0'+tr.out)<<(tr.dir==1?'R':'L')<<char('A'+tr.nxt);
			}
		}
		return os;
	}
	friend std::istream &operator>>(std::istream &is,TM &tm){
		std::string str;
		assert(is>>str);
		//std::cerr<<'"'<<str<<'"'<<'\n';
		//assert(str.length()>=N_STATE*N_CHAR*3+N_STATE-1);
		int64_t p=0;
		for(int64_t i=0;;++i){
			assert(i<N_STATE);
			/*if(i){
				assert(str[p]=='_');
				++p;
			}*/
			for(int64_t j=0;;++j){
				if(p>=(int64_t)str.length())return is;
				if(isspace(str[p]))return is;
				if(str[p]=='_'){
					++p;
					break;
				}
				assert(j<N_CHAR);
				Trans &tr=tm.trans[i][j];
				tm.n_state=std::max(tm.n_state,i+1);
				tm.n_char=std::max(tm.n_char,j+1);
				assert(p+3<=(int64_t)str.length());
				char o=str[p++];
				char d=str[p++];
				char s=str[p++];
				if(o=='-'&&d=='-'&&s=='-'){
					tr.nxt=-1;
					tr.dir=1;
					tr.out=1;
				}else{
					tr.nxt=s-'A';
					tr.dir=(d=='R'?1:d=='L'?-1:0);
					tr.out=o-'0';
					assert(0<=tr.nxt&&tr.nxt<N_STATE);
					assert(tr.dir*tr.dir==1);
					assert(0<=tr.out&&tr.out<N_CHAR);
				}
			}
		}
		return is;
	}
};

template<class T>
std::pair<std::vector<T>,T> pop_back(std::vector<T> w){
	T c=w.back();
	w.pop_back();
	return {w,c};
}
template<class T>
std::pair<T,std::vector<T>> pop_front(std::vector<T> w){
	T c=w.at(0);
	w.erase(w.begin());
	return {c,w};
}

struct WordLenError{};
#if 0
constexpr int64_t MAX_WORD_LENGTH=32;
struct Word{
	uint32_t data;
	int32_t len;
	Word(uint64_t d,int64_t n):data(d),len(n){
		assert(0<=len&&len<=MAX_WORD_LENGTH);
	}
	Word operator+(const Word &w)const{
		if(len+w.len>MAX_WORD_LENGTH)throw WordLenError();
		//assert(len+w.len<=64);
		return Word(data|(w.data<<len),len+w.len);
	}
	Word pop()const{return Word(data>>1,len-1);}
	std::pair<Word,Word> pop(int64_t n)const{
		assert(n<MAX_WORD_LENGTH);
		return {Word(data&((1uL<<n)-1uL),n),Word(data>>n,std::max(0L,len-n))};
	}
	Word push(uint8_t in)const{
		assert(in==0||in==1);
		return Word(data<<1|in,len+1);
	}
	uint8_t at(int64_t x)const{
		assert(0<=x&&x<len);
		return (data>>x)&1;
	}
	bool operator==(Word w)const{
		return data==w.data&&len==w.len;
	}
	bool operator!=(Word w)const{
		return !(*this==w);
	}
	bool operator<(Word w)const{
		return len!=w.len?len<w.len:data<w.data;
	}
	friend std::ostream &operator<<(std::ostream &os,Word w){
		for(int64_t i=0;i<w.len;++i)os<<(int64_t)w.at(i);
		return os;
	}
	size_t hash()const{
		return data+len*1237;
	}
	int64_t size()const{
		return len;
	}
	bool operator!()const{
		return !data;
	}
	uint64_t to_u64()const{return data;}
	
	chr_t to_chr()const{
		assert(len==log2_N_CHAR);
		return data;
	}
	Word at(int64_t x,int64_t n)const{
		return pop(x*n).second.pop(n).first;
	}
	Word del(int64_t x,int64_t n)const{
		auto [w0,w1]=pop(x*n);
		return w0+w1.pop(n).second;
	}
	Word set_chr(int64_t x,chr_t y){
		auto [w0,w1]=pop_chrs(x);
		return w0+Word(y,log2_N_CHAR)+w1.pop_chrs(1).second;
	}
	chr_t get_chr(int64_t x){
		return at(x,log2_N_CHAR).to_chr();
	}
	std::pair<Word,Word> pop_chrs(int64_t n)const{
		return pop(n*log2_N_CHAR);
	}
	static Word from_chr(chr_t x){
		return Word(x,log2_N_CHAR);
	}
};
#else
template<size_t n>
bool operator<(std::bitset<n> a,std::bitset<n> b){
	auto i=(a^b)._Find_first();
	return i<n&&b[i];
}
constexpr int64_t MAX_WORD_LENGTH=64*1;
struct Word{
	std::bitset<MAX_WORD_LENGTH> data;
	int64_t len;
	Word(std::bitset<MAX_WORD_LENGTH> d,int64_t n):data(d),len(n){
		assert(0<=len&&len<=MAX_WORD_LENGTH);
	}
	Word operator+(const Word &w)const{
		if(len+w.len>MAX_WORD_LENGTH)throw WordLenError();
		return Word(data|(w.data<<len),len+w.len);
	}
	Word set(int64_t x,int64_t y){
		assert(0<=x&&x<len);
		assert(0<=y&&y<2);
		auto d0=data;
		d0.set(x,y);
		return Word(d0,len);
	}
	Word pop()const{return Word(data>>1,len-1);}
	std::pair<Word,Word> pop(int64_t n)const{
		assert(n<MAX_WORD_LENGTH);
		return {Word(data<<(MAX_WORD_LENGTH-n)>>(MAX_WORD_LENGTH-n),n),Word(data>>n,std::max(0L,len-n))};
	}
	uint8_t at(int64_t x)const{
		assert(0<=x&&x<len);
		return data[x];
	}
	bool operator==(Word w)const{
		return data==w.data&&len==w.len;
	}
	bool operator!=(Word w)const{
		return !(*this==w);
	}
	bool operator<(Word w)const{
		return len!=w.len?len<w.len:data<w.data;
	}
	friend std::ostream &operator<<(std::ostream &os,Word w){
		for(int64_t i=0;i<w.len;++i)os<<(int64_t)w.at(i);
		return os;
	}
	size_t hash()const{
		return std::hash<std::bitset<MAX_WORD_LENGTH>>()(data)+len*1237;
	}
	int64_t size()const{
		return len;
	}
	bool operator!()const{
		return !data.any();
	}
	uint64_t to_u64()const{return data.to_ulong();}
	
	chr_t to_chr()const{
		assert(len==log2_N_CHAR);
		return data.to_ulong();
	}
	Word at(int64_t x,int64_t n)const{
		return pop(x*n).second.pop(n).first;
	}
	Word del(int64_t x,int64_t n)const{
		auto [w0,w1]=pop(x*n);
		return w0+w1.pop(n).second;
	}
	Word set_chr(int64_t x,chr_t y){
		auto [w0,w1]=pop_chrs(x);
		return w0+Word(y,log2_N_CHAR)+w1.pop_chrs(1).second;
	}
	chr_t get_chr(int64_t x){
		return at(x,log2_N_CHAR).to_chr();
	}
	std::pair<Word,Word> pop_chrs(int64_t n)const{
		return pop(n*log2_N_CHAR);
	}
	static Word from_chr(chr_t x){
		return Word(x,log2_N_CHAR);
	}
};
#endif

Word rev(Word w){
	assert(w.len%log2_N_CHAR==0);
	Word u(0,0);
	while(w.len){
		auto [w0,w1]=w.pop(log2_N_CHAR);
		w=w1;
		u=w0+u;
	}
	return u;
}
Word all0(int64_t len){
	return Word(0,len*log2_N_CHAR);
}
constexpr int64_t _block_maxT=320*10;
thread_local int64_t block_maxT;

struct WordUpdateLemma{
	Word w0,w1;
	int64_t s0,s1,sgn,n_step;
	bool is_back;
	// s0 w0 => w1 s1: is_back=0
	// s0 w0 => s1 w1: is_back=1
	static std::optional<WordUpdateLemma> from_v2(const TM &tm,const Word w,const int64_t s,const int64_t sgn,const int64_t maxT=_block_maxT){
		auto opt=from(tm,w,s,sgn,maxT);
		if(opt&&!opt->is_back)opt->w1=rev(opt->w1);
		return opt;
	}
	static std::optional<WordUpdateLemma> from(const TM &tm,const Word w,const int64_t s,const int64_t sgn,const int64_t maxT=_block_maxT){
		assert(sgn==-1||sgn==1);
		assert(w.len%log2_N_CHAR==0);
		auto len=w.len/log2_N_CHAR;
		if(s==-1)return std::nullopt;
		Word w1=w;
		int64_t s1=s,pos=0;
		//merr<<w1<<"  "<<char('A'+s)<<" pos="<<pos<<'\n';
		for(int64_t T_=0;T_<maxT;++T_){
			uint64_t in=w1.get_chr(pos);
			assert(in<N_CHAR);
			Trans tr=tm.trans[s1][in];
			//if(tr.nxt==-1)return std::nullopt;
			s1=tr.nxt;
			w1=w1.set_chr(pos,tr.out);
			pos+=tr.dir*sgn;
			//merr<<w1<<"  "<<char('A'+s1)<<" pos="<<pos<<'\n';
			if(s1==-1||pos<0||pos>=len){
				block_maxT=std::max(T_+1,block_maxT);
				return WordUpdateLemma{w,w1,s,s1,sgn,T_+1,pos<0};
			}
		}
		return std::nullopt;
	}
	friend std::ostream &operator<<(std::ostream &os,const WordUpdateLemma &w){
		os<<w.w0<<" => "<<w.w1<<"  "<<char('A'+w.s0)<<" => "<<char('A'+w.s1)<<" "<<"sgn:"<<w.sgn<<'\n';
		return os;
	}
};

template<>
std::ostream &operator<<(std::ostream &os,const std::vector<bool> &v){
	for(bool i:v)os<<int(i);
	return os;
}
template<class T>
std::vector<T> rev(std::vector<T> v){
	std::reverse(v.begin(),v.end());
	return v;
}
template<class T>
T pops1(std::vector<T> &xs){
	T c=0;
	if(!xs.empty()){
		c=xs.back();
		xs.pop_back();
	}
	return c;
}


struct DH_Tape{
	std::vector<chr_t> l,r;
	int64_t s=0,sgn=1;
	int64_t last_s=0,last_in=0;
	bool step(const TM &tm){
		if(s==-1)return 1;
		auto in=pops1(r);
		Trans tr=tm.trans[last_s=s][last_in=in];
		s=tr.nxt;
		if(tr.dir==sgn){
			l.push_back(tr.out);
		}else{
			r.push_back(tr.out);
			std::swap(l,r);
			sgn*=-1;
		}
		return 0;
	}
	std::optional<std::tuple<int64_t,int64_t,int64_t>> check_halt(const TM &tm,int64_t maxT){
		assert(s!=-1);
		for(int64_t i=1;i<=maxT;++i){
			if(step(tm))return std::make_tuple(i,last_s,last_in);
		}
		return std::nullopt;
	}
	friend std::ostream &operator<<(std::ostream &os,const DH_Tape &t){
		if(t.sgn==1){
			os<<t.l<<' '<<char('A'+t.s)<<"> "<<rev(t.r);
		}else{
			os<<t.r<<" <"<<char('A'+t.s)<<' '<<rev(t.l);
		}
		return os;
	}
};

struct DH_WordTape{
	std::vector<Word> l,r;
	int64_t s=0,sgn=1;
};

namespace ChrTransform{
thread_local int64_t len_h,len_h_no_lru,lru_n;
thread_local bool add_state_to_h;
thread_local int64_t max_bsz,mnc,mod;
void init(){
	len_h=0;
	len_h_no_lru=0;
	lru_n=1;
	add_state_to_h=1;
	max_bsz=0;
	mnc=0;
	mod=1;
}
Word upd(Word w1,Word w0,int64_t s,int64_t sgn){
	//std::cerr<<"upd "<<w1<<" "<<w0<<" "<<s<<" "<<sgn<<'\n';
	if(len_h){
		//if(sgn==-1)w0=rev(w0);
		if(add_state_to_h){
			assert(0<=s&&s<N_STATE);
			assert(w0.len%log2_N_CHAR==0);
			w0=Word(s,log2_N_STATE)+(w0.len==log2_N_CHAR?w0:(Word(sgn==-1,1)+w0));
		}
		w1=w0+w1;
		int64_t len=w0.len;
		assert(w1.len%len==0);
		if(w1.len>len_h_no_lru*len){
			w0=w1.at(len_h_no_lru,len);
			for(int64_t i=len_h_no_lru+1,j=1;i*len<w1.len;++i){
				if(w1.at(i,len)==w0){
					if(j<lru_n){
						++j;
						continue;
					}
					w1=w1.del(i,len);
					break;
				}
			}
		}
		if(w1.len>len_h*len)w1=w1.pop(len_h*len).first;
		if(max_bsz>0&&mnc>=0&&mod>0){
			for(int64_t bsz=1;bsz<=max_bsz;++bsz){
				if(bsz*len*(mnc+mod)>w1.len)break;
				auto w2=w1.pop(bsz*len*(mnc+mod)).first;
				if(w2.pop(bsz*len).second==w2.pop(w2.len-bsz*len).first){
					w1=w2.pop(bsz*len*mod).second;
					break;
				}
			}
		}
	}
	//std::cerr<<"= "<<w1<<'\n';
	return w1;
}

}
using ChrTransform::upd;



struct TM_t{
	virtual Word zero()const=0;
	virtual std::optional<WordUpdateLemma> operator()(Word w0,int64_t s,int64_t sgn)const=0;
	virtual DH_WordTape from(const DH_Tape &t)const=0;
	virtual void print(std::ostream &os)const=0;
	virtual int64_t n_state()const=0;
	friend std::ostream &operator<<(std::ostream &os,const TM_t &tm){
		tm.print(os);
		return os;
	}
};

struct BlockTM:TM_t{
	const TM &tm;
	size_t len;
	BlockTM(const TM &tm,size_t len):tm(tm),len(len){}
	Word zero()const override{return all0(len);}
	int64_t n_state()const override{return N_STATE;}
	std::optional<WordUpdateLemma> operator()(Word w,int64_t s,int64_t sgn)const override{
		auto [w0,w1]=w.pop_chrs(len);
		auto res=WordUpdateLemma::from_v2(tm,w0,s,sgn);
		if(res){
			w1=upd(w1,w0,s,sgn);
			res->w1=res->w1+w1;
		}
		return res;
	}
	std::vector<Word> from(std::vector<chr_t> w)const{
		std::reverse(w.begin(),w.end());
		while(w.size()%len)w.emplace_back(0);
		std::reverse(w.begin(),w.end());
		std::vector<Word> ls;
		size_t sz=w.size();
		for(size_t i=0;i<sz;i+=len){
			Word w0=all0(len);
			for(size_t j=0;j<len;++j){
				w0=w0.set_chr(j,w[i+len-1-j]);
			}
			ls.emplace_back(w0);
		}
		return ls;
	}
	DH_WordTape from(const DH_Tape &t)const override{
		DH_WordTape ret;
		ret.l=from(t.l);
		ret.r=from(t.r);
		ret.s=t.s;
		ret.sgn=t.sgn;
		return ret;
	}
	void print(std::ostream &os)const override{
		os<<tm<<" n="<<len;
	}
};

struct FoldTapeTM:TM_t{
	BlockTM tm_l,tm_r;
	FoldTapeTM(const TM &tm,size_t len_l,size_t len_r):tm_l(tm,len_l),tm_r(tm,len_r){}
	Word zero()const override{return tm_l.zero()+tm_r.zero();}
	int64_t n_state()const override{return N_STATE*2;}
	std::optional<WordUpdateLemma> operator()(Word w,int64_t s,int64_t sgn)const override{
		//if(dbg)merr<<w.len<<"qwq\n";
		if(w.len>1){
			auto [wl,t1]=w.pop_chrs(tm_l.len);
			auto [wr,t2]=t1.pop_chrs(tm_r.len);
			std::optional<WordUpdateLemma> res;
			if(s&1){
				res=tm_l(wl+t2,s/2,-sgn);
				if(!res)return std::nullopt;
				std::tie(wl,t2)=res->w1.pop_chrs(tm_l.len);
				res->w1=wl+wr+t2;
			}else{
				res=tm_r(wr+t2,s/2,sgn);
				if(!res)return std::nullopt;
				res->w1=wl+res->w1;
			}
			res->w0=w;
			res->s0=s;
			if(res->s1!=-1)res->s1=res->s1*2+(s&1);
			res->sgn=sgn;
			//if(dbg)merr<<res->w0<<"_"<<res->s0<<"_"<<res->sgn<<" => "<<res->w1<<"_"<<res->s1<<'\n';
			return res;
		}else{
			assert(w==Word(1,1));
			return WordUpdateLemma{w,w,s,s^1,sgn,0,1};
		}
	}
	DH_WordTape from(const DH_Tape &t)const override{
		assert(t.l.empty());
		assert(t.r.empty());
		assert(t.s==0);
		assert(t.sgn==1);
		DH_WordTape ret;
		ret.l.emplace_back(1,1);
		ret.s=0;
		ret.sgn=1;
		return ret;
	}
	void print(std::ostream &os)const override{
		os<<tm_l.tm<<" fold("<<tm_l.len<<','<<tm_r.len<<')';
	}
};

namespace CTL_Fixedlen_verify{
struct DFA{
	std::map<std::pair<Word,size_t>,size_t> push_;
	std::unordered_map<size_t,std::set<std::pair<Word,size_t>>> pop_;
	size_t push(Word w,size_t ls){
		return push_[{w,ls}];
	}
	size_t push_all(const std::vector<Word> &ws,size_t ls){
		for(Word w:ws){
			ls=push(w,ls);
			if(!ls)return 0;
		}
		return ls;
	}
	DFA(std::map<std::pair<Word,size_t>,size_t> push_0):push_(push_0){
		for(auto [k,v]:push_)pop_[v].emplace(k);
	}
};
struct Verifier{
	typedef std::tuple<size_t,size_t,int64_t,int64_t> Tape;
	DFA lset,rset;
	std::set<Tape> q;
	size_t l0,r0;
	Verifier(const std::map<std::pair<Word,size_t>,size_t> &lset0,const std::map<std::pair<Word,size_t>,size_t> &rset0,const std::set<Tape> &q0,size_t l0,size_t r0):lset(lset0),rset(rset0),q(q0),l0(l0),r0(r0){}
	bool verify(const TM_t &tm,const DH_WordTape &t0){
		block_maxT=0;
		bool flag=0;
		if(!(lset.push(tm.zero(),l0)==l0))return 0;
		if(!(rset.push(tm.zero(),r0)==r0))return 0;
		auto l1=lset.push_all((t0.sgn==1?t0.l:t0.r),l0);
		auto r1=rset.push_all((t0.sgn==1?t0.r:t0.l),r0);
		if(t0.sgn==-1)std::swap(l1,r1);
		if(!q.count(Tape{l1,r1,t0.s,t0.sgn}))return 0;
		for(auto [l,r,s,sgn]:q){
			if(!l)return 0;
			if(!r)return 0;
			if(!(0<=s&&s<tm.n_state()))return 0;
			if(!(sgn==1||sgn==-1))return 0;
			for(auto [w,r1]:(sgn==1?rset:lset).pop_[r]){
				auto res=tm(w,s,sgn);
				if(!res)return 0;
				if(res->s1==-1)return 0;
				if(res->is_back){
					Tape t{(sgn==1?rset:lset).push(res->w1,r1),l,res->s1,-sgn};
					if(!q.count(t))return 0;
				}else{
					Tape t{(sgn==1?lset:rset).push(res->w1,l),r1,res->s1,sgn};
					if(!q.count(t))return 0;
				}
			}
		}
		if(verbose)
		std::cerr<<"verified:"
		<<" lset:"<<lset.pop_.size()<<" lset_tr:"<<lset.push_.size()
		<<" rset:"<<rset.pop_.size()<<" rset_tr:"<<rset.push_.size()
		<<" accept_set:"<<q.size()
		<<" block_maxT:"<<block_maxT
		<<'\n';
		return 1;
	}
};
}

template<class T>
struct IdAlloc{
	std::map<T,int64_t> id;
	std::vector<const T*> idr;
	int64_t get_id(const T &x){
		auto [it,flag]=id.emplace(x,idp());
		if(flag)idr.emplace_back(&(it->first));
		return it->second;
	}
	const T &at(size_t i)const{
		return *idr.at(i);
	}
	int64_t idp()const{
		return (int64_t)idr.size();
	}
};

namespace MitM_CTL{

struct LRUpair{
	inline static thread_local int64_t len_h,len_h_no_lru,len_h_tail;
	static void init(){
		len_h=8,len_h_no_lru=2,len_h_tail=0;
	}
	std::vector<Word> q;
	std::vector<std::pair<Word,Word>> lru;
	LRUpair(int64_t sgn){}
	bool operator<(const LRUpair &w)const{
		return q!=w.q?q<w.q:lru<w.lru;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		q.emplace_back(w);
		if(q.size()<=len_h_no_lru+len_h_tail)return;
		if(len_h_tail&&len_h-len_h_no_lru-len_h_tail>0){
			auto p=std::make_pair(q[len_h_tail-1],q[len_h_tail]);
			auto it=std::find(lru.begin(),lru.end(),p);
			if(it==lru.end()&&lru.size()>len_h-len_h_no_lru-len_h_tail)it=lru.end()-1;
			if(it!=lru.end())lru.erase(it);
			lru.insert(lru.begin(),p);
		}
		q.erase(q.begin()+len_h_tail);
	}
	friend std::ostream &operator<<(std::ostream &os,const LRUpair &x){
		return os<<x.q<<x.lru;
	}
};

struct set_pair{
	inline static thread_local int64_t len_h,len_h_no_lru,len_h_tail;
	static void init(){
		len_h=16,len_h_no_lru=2,len_h_tail=1;
	}
	std::vector<Word> q;
	std::vector<std::pair<Word,Word>> lru;
	set_pair(int64_t sgn){}
	bool operator<(const set_pair &w)const{
		return q!=w.q?q<w.q:lru<w.lru;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		q.emplace_back(w);
		if(q.size()<=len_h_no_lru+len_h_tail)return;
		if(len_h_tail&&len_h-len_h_no_lru-len_h_tail>0){
			auto p=std::make_pair(q[len_h_tail-1],q[len_h_tail]);
			auto it=std::find(lru.begin(),lru.end(),p);
			if(it==lru.end()){
				lru.emplace_back(p);
				std::sort(lru.begin(),lru.end());
				if(lru.size()>len_h)throw WordLenError();
			}
		}
		q.erase(q.begin()+len_h_tail);
	}
	friend std::ostream &operator<<(std::ostream &os,const set_pair &x){
		return os<<x.q<<x.lru;
	}
};

struct NGset{
	inline static thread_local int64_t NG_n,len_h;
	static void init(){
		NG_n=3,len_h=64;
	}
	Word q=Word(0,0);
	std::vector<Word> lru;
	NGset(int64_t sgn){}
	bool operator<(const NGset &w)const{
		return q!=w.q?q<w.q:lru<w.lru;
	}
	void push(Word w){
		if(!q.len&&!w)return;
		auto q0=q;
		q=w+q;
		if(q.len>NG_n*w.len){
			q=q.pop(q0.len).first;
			auto it=std::lower_bound(lru.begin(),lru.end(),q0);
			if(it==lru.end()||*it!=q0){
				lru.insert(it,q0);
				if(lru.size()>len_h)throw WordLenError();
			}
		}
	}
	friend std::ostream &operator<<(std::ostream &os,const NGset &x){
		return os<<x.q<<x.lru;
	}
};

struct NG1{
	inline static thread_local int64_t NG_n;
	static void init(){
		NG_n=3;
	}
	Word q=Word(0,0);
	NG1(int64_t sgn){}
	bool operator<(const NG1 &w)const{
		return q<w.q;
	}
	void push(Word w){
		if(!q.len&&!w)return;
		auto q0=q;
		q=w+q;
		if(q.len>NG_n*w.len){
			q=q.pop(q0.len).first;
		}
	}
	friend std::ostream &operator<<(std::ostream &os,const NG1 &x){
		return os<<x.q;
	}
};

struct NG{
	inline static thread_local int64_t NG_n,tH,pos_mod;
	static void init(){
		NG_n=3;
		tH=0;
		pos_mod=1;
	}
	std::vector<Word> q,q0;
	int64_t mod=0;
	NG(int64_t sgn){}
	bool operator<(const NG &w)const{
		return mod!=w.mod?mod<w.mod:q!=w.q?q<w.q:q0<w.q0;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		if(q.size()==NG_n&&NG_n)q.erase(q.begin());
		q.emplace_back(w);
		if(q0.size()<tH)q0.emplace_back(w);
		mod=(mod+1)%pos_mod;
	}
	friend std::ostream &operator<<(std::ostream &os,const NG &x){
		return os<<x.q<<x.q0<<'_'<<x.mod;
	}
};

/*struct Test{
	inline static thread_local int64_t LH,RH;
	static void init(){
		LH=54;
		RH=32;
	}
	Test(int64_t sgn):sgn(sgn){}
	int64_t sgn;
	std::vector<bool> q,q0;
	bool operator<(const Test &w)const{
		assert(sgn==w.sgn);
		return q!=w.q?q<w.q:q0<w.q0;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		size_t H,tH;
		if(sgn==-1)tH=LH,H=RH;
		else tH=RH,H=LH;
		if(q.size()>=H&&H)q.erase(q.begin());
		q.emplace_back(!!w);
		if(q0.size()<tH)q0.emplace_back(!!w);
	}
	friend std::ostream &operator<<(std::ostream &os,const Test &x){
		return os<<x.q<<x.q0;
	}
};*/
/*
struct Test{
	static void init(){}
	Test(int64_t sgn):sgn(sgn){}
	int64_t sgn,tp=0;
	std::vector<Word> q;
	bool operator<(const Test &w)const{
		assert(sgn==w.sgn);
		return tp!=w.tp?tp<w.tp:q<w.q;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		if(sgn==-1){
			if(tp==0){
				q.emplace_back(w);
				if(q.size()==3)tp=1;
			}else if(tp==1){
				q.emplace_back(w);
				if(q.size()==6){
					if(q[3]==Word(4,3)&&q[4]==Word(2,3)&&q[5]==Word(2,3)){
						q.resize(3);
					}else{
						tp=2;
					}
				}
			}else if(tp==2){
				q.emplace_back(w);
				if(q.size()>=10){
					q.erase(q.end()-2);
				}
			}
		}else{
			q.emplace_back(w);
			if(q.size()>=6)q.erase(q.begin());
		}
	}
	friend std::ostream &operator<<(std::ostream &os,const Test &x){
		return os<<x.sgn<<' '<<x.tp<<':'<<x.q;
	}
};*/

struct NestRep{
	inline static thread_local int64_t H,tH,dep;
	inline static thread_local uint16_t mnc,mod;
	typedef std::tuple<uint32_t,uint16_t,uint16_t> Rep;
	typedef IdAlloc<std::variant<Word,std::pair<int64_t,std::vector<Rep>>>> IdAlloc_;
	inline static thread_local IdAlloc_ mp;
	static void init(){
		H=10;
		tH=10;
		dep=3;
		mnc=2;
		mod=1;
		mp=IdAlloc_();
	}
	static int64_t get_dep(uint32_t id){
		if(auto pvec=std::get_if<std::pair<int64_t,std::vector<Rep>>>(&mp.at(id)))return pvec->first;
		return 0;
	}
	static bool maintain0(std::vector<Rep> &q){
		size_t sz=q.size();
		for(size_t i=1;i*2<sz;++i){
			bool flag=1;
			for(size_t j=0;j<i;++j){
				if(q[sz-i*2+j]!=q[sz-i+j]){flag=0;break;}
			}
			if(!flag)continue;
			int64_t dep_=0;
			for(size_t j=0;j<i;++j){
				dep_=std::max(dep_,1+get_dep(std::get<0>(q[sz-i+j])));
				if(dep_>=dep){flag=0;break;}
			}
			if(!flag)continue;
			auto id=mp.get_id(std::make_pair(dep_,std::vector<Rep>(q.end()-i,q.end())));
			q.resize(sz-i*2);
			q.emplace_back(id,std::min<uint16_t>(2,mnc),2%mod);
			return 1;
		}
		for(size_t i=sz-2;i<sz;--i){
			auto pvec=std::get_if<std::pair<int64_t,std::vector<Rep>>>(&mp.at(std::get<0>(q[i])));
			if(!pvec)continue;
			auto &vec=pvec->second;
			if(i+1+vec.size()!=sz)continue;
			bool flag=1;
			for(size_t j=0;j<vec.size();++j){
				if(q[i+1+j]!=vec[j]){flag=0;break;}
			}
			if(!flag)continue;
			q.resize(i+1);
			auto &[_,mnc_,mod_]=q[i];
			mnc_=std::min<uint16_t>(mnc_+1,mnc);
			mod_=(mod_+1)%mod;
			return 1;
		}
		return 0;
	}
	static void maintain(std::vector<Rep> &q){
		while(maintain0(q));
	}
	NestRep(int64_t sgn){}
	std::vector<Rep> q,q0;
	bool operator<(const NestRep &w)const{
		return q!=w.q?q<w.q:q0<w.q0;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		assert(H>0);
		Rep w_(mp.get_id(w),1,1%mod);
		q.emplace_back(w_);
		maintain(q);
		if(q.size()>H)q.erase(q.begin());
		if(q0.size()<tH){
			q0.emplace_back(w_);
			maintain(q0);
		}
	}
	struct PrintNestRep{
		const std::vector<Rep> &q;
		bool rev;
		PrintNestRep(const std::vector<Rep> &q,bool rev):q(q),rev(rev){}
		friend std::ostream &operator<<(std::ostream &os,const PrintNestRep &x){
			os<<'(';
			bool flag=0;
			auto q=x.q;
			if(x.rev)std::reverse(q.begin(),q.end());
			for(auto &[id,mnc_,mod_]:q){
				if(flag)os<<' ';
				flag=1;
				auto &w=mp.at(id);
				if(auto pvec=std::get_if<std::pair<int64_t,std::vector<Rep>>>(&w)){
					os<<PrintNestRep(pvec->second,x.rev);
				}else if(auto pw=std::get_if<Word>(&w)){
					os<<*pw;
				}else assert(0);
				if(mnc_>1){
					os<<"^";
					if(mnc_>=mnc)os<<"%"<<mod_;
					else os<<mnc_;
				}
			}
			os<<')';
			return os;
		}
	};
	friend std::ostream &operator<<(std::ostream &os,const NestRep &x){
		return os<<PrintNestRep(x.q,0)<<" q0["<<PrintNestRep(x.q0,0)<<"]";
	}
};

struct RWL_mod{
	inline static thread_local int64_t len_h,len_h_tail;
	inline static thread_local int32_t mnc,mod;
	static void init(){
		len_h=8,len_h_tail=0,mnc=2,mod=1;
	}
	typedef std::tuple<Word,int32_t,int32_t> RepeatWord;
	std::vector<RepeatWord> q;
	RWL_mod(int64_t sgn){}
	bool operator<(const RWL_mod &w)const{
		return q<w.q;
	}
	void push(Word w){
		if(q.empty()){
			if(!!w)q.emplace_back(w,1,1);
			return;
		}
		auto &[w0,n,m]=q.back();
		if(w0!=w){
			q.emplace_back(w,1,1);
			/*if(q.size()>len_h_no_lru+len_h_tail){
				auto it0=q.end()-len_h_no_lru-1;
				auto it=std::find(q.begin()+len_h_tail,it0,*it0);
				if(it!=it0)q.erase(it);
			}*/
			if(q.size()>len_h)q.erase(q.begin()+len_h_tail);
			return;
		}
		n=std::min(n+1,mnc);
		m=(m+1)%mod;
	}
	friend std::ostream &operator<<(std::ostream &os,const RWL_mod &x){
		return os<<x.q;
	}
};
struct RWS_mod{
	inline static thread_local int64_t len_h,len_h_tail;
	inline static thread_local int32_t mnc,mod;
	static void init(){
		len_h=8,mnc=2,mod=1;
	}
	typedef std::tuple<Word,int32_t,int32_t> RepeatWord;
	std::vector<RepeatWord> q;
	RWS_mod(int64_t sgn){}
	bool operator<(const RWS_mod &w)const{
		return q<w.q;
	}
	void push(Word w){
		if(q.empty()){
			if(!!w)q.emplace_back(w,1,1);
			return;
		}
		for(size_t i=0;i<q.size();++i){
			auto &[w0,n,m]=q[i];
			if(w0==w){
				n=std::min(n+1,mnc);
				m=(m+1)%mod;
				q.emplace_back(q[i]);
				q.erase(q.begin()+i);
				return;
			}
		}
		q.emplace_back(w,1,1);
		if(q.size()>len_h)q.erase(q.begin());
	}
	friend std::ostream &operator<<(std::ostream &os,const RWS_mod &x){
		return os<<x.q;
	}
};
struct RNGS_mod{
	inline static thread_local int64_t NG_n,len_h,len_h_tail,bs_n;
	inline static thread_local int32_t mnc,mod;
	static void init(){
		NG_n=4;
		len_h=8,mnc=2,mod=1;
		bs_n=0;
	}
	typedef std::tuple<Word,int32_t,int32_t> RepeatWord;
	std::vector<RepeatWord> q;
	std::vector<Word> q0;
	Word w1=Word(0,0);
	RNGS_mod(int64_t sgn){}
	bool operator<(const RNGS_mod &w)const{
		return w1!=w.w1?w1<w.w1:q0!=w.q0?q0<w.q0:q<w.q;
	}
	void push(Word w){
		if(w1.len==0&&!w)return;
		w1=w=(w+w1).pop(NG_n).first;
		q0.push_back(w);
		if(q0.size()>bs_n){
			w=q0.front();
			q0.erase(q0.begin());
		}else return;
		for(size_t i=0;i<q.size();++i){
			auto &[w0,n,m]=q[i];
			if(w0==w){
				n=std::min(n+1,mnc);
				m=(m+1)%mod;
				q.emplace_back(q[i]);
				q.erase(q.begin()+i);
				return;
			}
		}
		q.emplace_back(w,1,1);
		if(q.size()>len_h)q.erase(q.begin());
	}
	friend std::ostream &operator<<(std::ostream &os,const RNGS_mod &x){
		return os<<x.w1<<","<<x.q0<<","<<x.q;
	}
};
struct RS_mod{
	inline static thread_local int64_t NG_n,len_h,len_h_tail;
	inline static thread_local int32_t mnc,mod;
	inline static thread_local bool is_strict;
	static void init(){
		NG_n=4;
		len_h=8,mnc=2,mod=1;
		is_strict=1;
	}
	typedef std::tuple<Word,int32_t,int32_t> RepeatWord;
	std::vector<RepeatWord> q;
	std::vector<Word> q0;
	RS_mod(int64_t sgn){}
	bool operator<(const RS_mod &w)const{
		return q0!=w.q0?q0<w.q0:q<w.q;
	}
	void push(Word w){
		if(q0.empty()&&!w)return;
		q0.push_back(w);
		if(q0.size()>NG_n){
			w=q0.front();
			q0.erase(q0.begin());
		}else return;
		for(size_t i=0;i<q.size();++i){
			auto &[w0,n,m]=q[i];
			if(w0==w){
				n=std::min(n+1,mnc);
				m=(m+1)%mod;
				q.emplace_back(q[i]);
				q.erase(q.begin()+i);
				return;
			}
		}
		q.emplace_back(w,1,1);
		if(q.size()>len_h){
			if(is_strict)throw WordLenError();
			q.erase(q.begin());
		}
	}
	friend std::ostream &operator<<(std::ostream &os,const RS_mod &x){
		return os<<x.q;
	}
};
struct RndDFA{
	inline static thread_local int64_t len_h,len_h_tail,seed,max_state_num;
	inline static thread_local int32_t mnc,mod;
	inline static thread_local std::mt19937 mt;
	struct DFA{
		std::map<std::pair<Word,int32_t>,int32_t> tr;
		int32_t p;
		DFA():p(1){}
		int32_t get(Word w,int32_t cur,std::mt19937 &mt){
			int32_t &nxt=tr[std::make_pair(w,cur)];
			if(!nxt){
				nxt=mt()%(max_state_num+1);
				if(nxt>p)nxt=++p;
			}
			return nxt;
		}
	};
	inline static thread_local DFA dfa[2];
	static void init(){
		len_h=8,mnc=2,mod=1;
		dfa[0]=DFA();
		dfa[1]=DFA();
		max_state_num=32;
		seed=0;
	}
	static void init2(){
		mt=std::mt19937(seed);
	}
	typedef std::tuple<std::pair<Word,int32_t>,int32_t,int32_t> RepeatWord;
	std::vector<RepeatWord> q;
	int64_t sgn;
	RndDFA(int64_t sgn):sgn(sgn){}
	bool operator<(const RndDFA &w)const{
		assert(sgn==w.sgn);
		return q<w.q;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		int32_t cur=1;
		if(!q.empty()){
			auto [a,b]=std::get<0>(q.back());
			cur=dfa[sgn<0].get(a,b,mt);
		}
		auto w_=std::make_pair(w,cur);
		for(size_t i=0;i<q.size();++i){
			auto &[w0,n,m]=q[i];
			if(w0==w_){
				n=std::min(n+1,mnc);
				m=(m+1)%mod;
				q.emplace_back(q[i]);
				q.erase(q.begin()+i);
				return;
			}
		}
		q.emplace_back(w_,1,1);
		if(q.size()>len_h)q.erase(q.begin());
	}
	friend std::ostream &operator<<(std::ostream &os,const RndDFA &x){
		return os<<x.q;
	}
};



struct ExRWL{
	inline static thread_local int64_t len_h,len_h_tail,block_size;
	static void init(){
		len_h=8,len_h_tail=0,block_size=1;
	}
	std::vector<Word> q;
	ExRWL(int64_t sgn){}
	bool operator<(const ExRWL &w)const{
		return q<w.q;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		if(q.empty()){
			q.emplace_back(w);
			return;
		}
		q.back()=w+q.back();
		if(q.back().len==block_size){
			if(q.size()>=2&&*(q.end()-2)==q.back()){
				q.pop_back();
				q.push_back(Word(0,0));
			}
		}else if(q.back().len>=block_size*2){
			auto t0=q.back();
			auto [h1,t1]=t0.pop(block_size);
			auto [h2,t2]=t1.pop(block_size);
			if(h1==h2){
				q.pop_back();
				q.push_back(t2);
				q.push_back(h1);
				q.push_back(Word(0,0));
			}/*else{
				q.back()=t0.pop(block_size*2).first;
			}*/
		}
		/*if(q.size()>len_h_no_lru+len_h_tail){
			auto it0=q.end()-len_h_no_lru-1;
			auto it=std::find(q.begin()+len_h_tail,it0,*it0);
			if(it!=it0)q.erase(it);
		}*/
		if(q.size()>len_h)q.erase(q.begin()+len_h_tail);
	}
	friend std::ostream &operator<<(std::ostream &os,const ExRWL &x){
		return os<<x.q;
	}
};


struct CPS_LRU{
	inline static thread_local int64_t len_h,len_h_no_lru,len_h_tail;
	static void init(){
		len_h=8,len_h_no_lru=2,len_h_tail=0;
	}
	std::vector<Word> ls_;
	CPS_LRU(int64_t sgn){}
	bool operator<(const CPS_LRU &w)const{
		return ls_<w.ls_;
	}
	void push(Word w){
		if(ls_.empty()&&!w)return;
		ls_.insert(ls_.begin(),w);
		if(ls_.size()<=len_h_no_lru)return;
		if(len_h_no_lru+1>(int64_t)ls_.size()-len_h_tail)return;
		auto it=std::find(ls_.begin()+len_h_no_lru+1,ls_.end()-len_h_tail,ls_[len_h_no_lru]);
		if(it==ls_.end()-len_h_tail&&ls_.size()>len_h)it=ls_.end()-len_h_tail-1;
		if(it!=ls_.end()-len_h_tail)ls_.erase(it);
	}
	friend std::ostream &operator<<(std::ostream &os,const CPS_LRU &x){
		return os<<x.ls_;
	}
};
struct CPS_LRU_BD{
	inline static thread_local int64_t len_h,len_h_no_lru;
	static void init(){
		len_h=8,len_h_no_lru=2;
	}
	std::vector<Word> ls_,ls0;
	CPS_LRU_BD(int64_t sgn){}
	bool operator<(const CPS_LRU_BD &w)const{
		return ls_!=w.ls_?ls_<w.ls_:ls0<w.ls0;
	}
	void push(Word w){
		if(ls_.empty()&&!w)return;
		if(ls0.size()<len_h_no_lru){
			ls0.push_back(w);
		}else if(ls0.size()<len_h){
			auto it=std::find(ls0.begin()+len_h_no_lru,ls0.end(),w);
			if(it==ls0.end())ls0.push_back(w);
		}
		ls_.insert(ls_.begin(),w);
		if(ls_.size()<=len_h_no_lru)return;
		auto it=std::find(ls_.begin()+len_h_no_lru+1,ls_.end(),ls_[len_h_no_lru]);
		if(it==ls_.end()&&ls_.size()>len_h)it=ls_.end()-1;
		if(it!=ls_.end())ls_.erase(it);
	}
	friend std::ostream &operator<<(std::ostream &os,const CPS_LRU_BD &x){
		return os<<x.ls_<<x.ls0;
	}
};



struct unique_wall{
	inline static thread_local int64_t len_h,len_h_no_lru,len_h_tail,mnc,sorted;
	static void init(){
		len_h=64,len_h_no_lru=0,len_h_tail=0,mnc=2,sorted=0;
	}
	std::vector<std::pair<Word,bool>> q;
	unique_wall(int64_t sgn){}
	bool operator<(const unique_wall &w)const{
		return q<w.q;
	}
	void push(Word w){
		if(q.empty()&&!w)return;
		q.emplace_back(w,false);
		int64_t sz=q.size();
		if(sz>len_h)throw WordLenError();
		if(sz<=len_h_tail+len_h_no_lru)return;
		auto it0=q.begin()+len_h_tail;
		auto it1=q.end()-1-len_h_no_lru;
		bool flag=0;
		size_t cnt=0;
		for(auto p=it0;p!=it1;++p){
			if(p->first==it1->first){
				if(p->second){
					flag=1;
				}else{
					++cnt;
				}
			}
		}
		if(flag){
			it1->second=true;
			assert(!cnt);
		}else if(cnt+1>=mnc){
			it1->second=true;
			for(auto p=it0;p!=it1;++p){
				if(p->first==it1->first){
					p->second=true;
				}
			}
		}else return;
		//rebuild lru
		std::set<Word> st;
		std::vector<std::pair<Word,bool>> q0;
		if(sorted){
			auto f=[&]{
				for(Word w:st)q0.emplace_back(w,true);
				st.clear();
			};
			for(auto p=q.rbegin();p!=q.rend();++p){
				if(!p->second){
					f();
					q0.emplace_back(*p);
				}else{
					st.insert(p->first);
				}
			}
			f();
		}else{
			for(auto p=q.rbegin();p!=q.rend();++p){
				if(!p->second){
					q0.emplace_back(*p);
					st.clear();
				}else if(st.insert(p->first).second){
					q0.emplace_back(*p);
				}
			}
		}
		std::reverse(q0.begin(),q0.end());
		q=std::move(q0);
	}
	friend std::ostream &operator<<(std::ostream &os,const unique_wall &x){
		return os<<x.q;
	}
};


template<class DFA_state_t>
struct Decider{
	typedef std::tuple<size_t,size_t,int64_t,int64_t> Tape;
	struct DFA{
		std::map<DFA_state_t,size_t> id;
		std::vector<const DFA_state_t*> idr;

		std::unordered_map<size_t,std::vector<std::pair<Word,size_t>>> pop_;
		std::map<std::pair<Word,size_t>,size_t> push_;
		
		size_t n_upd=0;
		bool upd_flag=0;
		std::vector<std::pair<size_t,std::pair<Word,size_t>>> upds;
		size_t push(Word w,size_t ls){
			//std::cerr<<"push "<<w<<" "<<ls<<" "<<idr.size()-1<<'\n';
			auto &ret=push_[{w,ls}];
			if(!ret){
				auto ls_=*idr.at(ls);
				ls_.push(w);
				auto ls0=get_id(ls_);
				ret=ls0;
				pop_[ls0].emplace_back(w,ls);
				upd_flag=1;
				++n_upd;
				upds.emplace_back(ls0,std::make_pair(w,ls));
			}
			return ret;
		}
		size_t push_all(const std::vector<Word> &ws,size_t ls){
			for(Word w:ws){
				ls=push(w,ls);
				if(!ls)return 0;
			}
			return ls;
		}
		
		size_t get_id(const DFA_state_t &ls){
			auto [it,flag]=id.emplace(ls,idr.size());
			if(flag)idr.emplace_back(&it->first);
			return it->second;
		}
		void init(const TM_t &tm,int64_t sgn){
			assert(idr.empty());
			idr.emplace_back();
			auto id0=get_id(DFA_state_t(sgn)),id1=id0;
			do id1=push(tm.zero(),id1); while(id1!=id0);
		}
		friend std::ostream &operator<<(std::ostream &os,const DFA &x){
			for(size_t i=1;i<x.idr.size();++i){
				merr<<*x.idr.at(i)<<":\n";
				if(x.pop_.count(i))
				for(auto [w,r1]:x.pop_.at(i)){
					merr<<w<<" => "<<*x.idr.at(r1)<<'\n';
				}
			}
			return os;
		}
	}lset,rset;
	std::deque<Tape> q;
	//std::set<Tape> inq;
	int64_t used_T=0;
	
	std::map<std::pair<size_t,int64_t>,std::set<Tape>> inqs;
	void push(Tape t){
		auto [l,r,s,sgn]=t;
		if(inqs[{r,sgn}].insert(t).second)q.push_back(t);
	}
	
	void print_Tape(Tape t){
		auto [l,r,s,sgn]=t;
		if(sgn==-1)std::swap(l,r);
		merr<<*lset.idr.at(l);
		merr<<" ";
		if(sgn==-1)merr<<"<";
		merr<<char('A'+s);
		if(sgn==1)merr<<">";
		merr<<" ";
		merr<<*rset.idr.at(r);
	}
	#define dbg1 dbg
	std::optional<std::list<Tape>> step(const TM_t &tm,Tape t,std::pair<Word,size_t> rs){
		//std::cerr<<"step "<<t<<rs<<'\n';
		std::list<Tape> ls;
		auto [l,r,s,sgn]=t;
		//auto rs=(sgn==1?rset:lset).pop_[r];
		if(dbg){
			merr<<"step: ";
			print_Tape(t);
			merr<<"\n";
		}
		auto [w,r1]=rs;
		//if(dbg)merr<<"step: "<<w<<" "<<(sgn==1?rset:lset).idr.at(r1)<<"\n";
		auto res=tm(w,s,sgn);
		if(!res){
			if(dbg1)merr<<w<<char('A'+s)<<sgn<<"block timeout\n"<<used_T<<'\n';
			return std::nullopt;
		}
		if(res->s1==-1){
			if(dbg1)merr<<*res<<"may halt\n"<<used_T<<'\n';
			return std::nullopt;
		}
		if(res->is_back){
			ls.emplace_back(Tape{(sgn==1?rset:lset).push(res->w1,r1),l,res->s1,-sgn});
		}else{
			ls.emplace_back(Tape{(sgn==1?lset:rset).push(res->w1,l),r1,res->s1,sgn});
		}
		return ls;
	}
	int64_t decide(const TM_t &tm,int64_t maxT,const DH_WordTape &tape0){
		if(to_coq)return 0;
		try{
			return decide_(tm,maxT,tape0);
		}catch(WordLenError e){
			return 0;
		}
	}
	int64_t decide_(const TM_t &tm,int64_t maxT,const DH_WordTape &tape0){
		lset.init(tm,-1);
		rset.init(tm,+1);
		auto l1=lset.push_all((tape0.sgn==1?tape0.l:tape0.r),1);
		auto r1=rset.push_all((tape0.sgn==1?tape0.r:tape0.l),1);
		if(tape0.sgn==-1)std::swap(l1,r1);
		push(Tape{l1,r1,tape0.s,tape0.sgn});
		for(;;){
			if(q.size()){
				Tape t=q.back();
				q.pop_back();
				auto [l,r,s,sgn]=t;
				auto rs=(sgn==1?rset:lset).pop_[r];
				for(auto rs0:rs){
					auto ls=step(tm,t,rs0);
					if(!ls)return 0;
					for(Tape &t0:*ls)push(t0);
				}
			}else if(!lset.upds.empty()){
				auto [id0,rs0]=lset.upds.back();
				lset.upds.pop_back();
				auto ts=inqs[{id0,-1}];
				for(auto t:ts){
					auto ls=step(tm,t,rs0);
					if(!ls)return 0;
					for(Tape &t0:*ls)push(t0);
				}
			}else if(!rset.upds.empty()){
				auto [id0,rs0]=rset.upds.back();
				rset.upds.pop_back();
				auto ts=inqs[{id0,1}];
				for(auto t:ts){
					auto ls=step(tm,t,rs0);
					if(!ls)return 0;
					for(Tape &t0:*ls)push(t0);
				}
			}else{
				std::set<Tape> inq;
				for(auto [k,v]:inqs)for(auto t:v)inq.insert(t);
				assert(CTL_Fixedlen_verify::Verifier(lset.push_,rset.push_,inq,1,1).verify(tm,tape0));
				if(verbose)std::cerr<<'\n'<<tm<<" N"<<" T:"<<used_T<<'\n';
				return 2;
			}
			++used_T;
			if(used_T+q.size()+lset.upds.size()+rset.upds.size()>=maxT){
				if(dbg1)std::cerr<<"timeout\n";
				return 0;
			}
			if(lset.n_upd+rset.n_upd>=maxT){
				if(dbg1)std::cerr<<"spaceout\n";
				return 0;
			}
		}
		return 0;
	}
	#undef dbg1
};
}


namespace FAR{

	#define dbg1 dbg
thread_local int64_t maxT;
thread_local int64_t used_S,used_T;
thread_local bool is_check=0;

void chk_S(){
	if(++used_S>=maxT){
		if(dbg1)merr<<"MLE\n"<<used_S<<'\n';
		throw int64_t(-3);
	}
}

void chk_T(){
	if(++used_T>=maxT*10){
		if(dbg1)merr<<"TLE\n"<<used_S<<'\n';
		throw int64_t(-4);
	}
}

template<class DFA_state_t>
struct Decider{
	std::map<DFA_state_t,size_t> id;
	std::vector<const DFA_state_t*> idr;

	std::unordered_map<size_t,std::vector<std::pair<Word,size_t>>> pop_;
	std::map<std::pair<Word,size_t>,size_t> push_;
	
	typedef std::pair<Word,size_t> DFA_edge;
	std::vector<std::pair<size_t,DFA_edge>> new_pops;
	size_t push(Word w,size_t ls){
		//std::cerr<<"push "<<w<<" "<<ls<<" "<<idr.size()-1<<'\n';
		auto &ret=push_[{w,ls}];
		if(!ret){
			assert(!is_check);
			auto ls_=*idr.at(ls);
			ls_.push(w);
			auto ls0=get_id(ls_);
			ret=ls0;
			chk_S();
			//pop_[ls0].emplace_back(w,ls);
			new_pops.emplace_back(ls0,std::make_pair(w,ls));
		}
		return ret;
	}
	
	size_t get_id(const DFA_state_t &ls){
		auto [it,flag]=id.emplace(ls,idr.size());
		if(flag)idr.emplace_back(&it->first);
		return it->second;
	}
	void init(){
		assert(idr.empty());
		idr.emplace_back();
		auto id0=get_id(DFA_state_t(+1));
		auto id1=push(tm.zero(),id0);
		assert(id0==1);
		assert(id0==id1);
	}
	
	typedef std::tuple<int64_t,int64_t> H2; // (QR,u) := QR> [u]
	typedef std::tuple<Word,int64_t,int64_t> H3; // (qR,QR,u) := qR QR> [u]
	typedef std::tuple<int64_t,int64_t> H2b; // (QL,u) := <QL [u]
	
	template<class K,class V>
	struct map{
		std::map<K,std::set<V>> mp;
		std::vector<std::pair<K,V>> todo;
		void add(K k,V v){
			chk_T();
			if(!mp[k].insert(v).second)return;
			assert(!is_check);
			chk_S();
			todo.emplace_back(k,v);
		}
		const std::set<V> &at(K k){
			return mp[k];
		}
		template<class F>
		bool on(const F &f){
			if(is_check){
				for(auto &[a,bs]:mp)for(auto b:bs)f(a,b);
				return 0;
			}
			if(todo.empty())return 0;
			auto [k,v]=todo.back();
			todo.pop_back();
			f(k,v);
			return 1;
		}
	};
	template<class K>
	struct set{
		std::set<K> st;
		std::vector<K> todo;
		void add(K k){
			chk_T();
			if(!st.insert(k).second)return;
			assert(!is_check);
			chk_S();
			todo.emplace_back(k);
		}
		bool count(K k){
			return st.count(k);
		}
		template<class F>
		bool on(const F &f){
			if(is_check){
				for(K k:st)f(k);
				return 0;
			}
			if(todo.empty())return 0;
			auto k=todo.back();
			todo.pop_back();
			f(k);
			return 1;
		}
	};
	map<H2,H2b> ret2;
	map<H3,H2b> ret3;
	map<H2,H3> pre23;
	map<H3,H2> pre32;
	map<H3,H3> pre33;
	set<H3> pre3L; // qR QR> [u] --> <QL [v] |- 0 <QL [v]
	set<H2b> retL;
	set<H3> H3s;
	set<H2> H2s;
	std::map<int64_t,std::set<int64_t>> r_s;
	const TM_t &tm;
	int64_t maxT=0;
	
	WordUpdateLemma tm_step(Word w,int64_t s,int64_t sgn){
		auto res=tm(w,s,sgn);
		if(!res){
			if(dbg1)merr<<w<<char('A'+s)<<sgn<<"block timeout\n"<<used_S<<" "<<used_T<<'\n';
			throw int64_t(-1);
		}
		if(res->s1==-1){
			if(dbg1)merr<<*res<<"may halt\n"<<used_S<<" "<<used_T<<'\n';
			throw int64_t(-2);
		}
		return *res;
	}
	
	void on_H2_pop(H2 a,DFA_edge b){
		auto [s,r]=a; // s> [r]
		auto [w,r0]=b; // s> w [r0]
		auto res=tm_step(w,s,+1);
		if(dbg1){
			merr<<"on_H2_pop\n";
			prH2(a);
			pr_DFA_edge(r,b);
		}
		if(res.is_back){// <s1 w1 [r0]
			if(dbg1)prH2b(H2b(res.s1,push(res.w1,r0)));
			ret2.add(a,H2b(res.s1,push(res.w1,r0)));
		}else{// w1 s1> [r0]
			H3 c(res.w1,res.s1,r0);
			if(dbg1)prH3(c);
			H3s.add(c);
			pre32.add(c,a);
		}
	}
	
	void on_H3_back(H3 c,H2b b){
		auto [w,s,r]=c; // w s> [r]
		auto [s0,r0]=b; // w <s0 [r0]
		auto res=tm_step(w,s0,-1);
		if(dbg1){
			merr<<"on_H3_back\n";
			prH3(c);
			merr<<w<<" ";
			prH2b(b);
		}
		if(res.is_back){// w1 s1> [r0]
			H3 c0(res.w1,res.s1,r0);
			H3s.add(c0);
			pre33.add(c0,c);
			if(dbg1)prH3(c0);
		}else{// <s1 w1 [r0]
			ret3.add(c,H2b(res.s1,push(res.w1,r0)));
			if(dbg1){
				pr_DFA_edge(push(res.w1,r0),{res.w1,r0});
				prH2b(H2b(res.s1,push(res.w1,r0)));
			}
		}
	}
	
	void on_retL(H2b b){
		auto [s0,r0]=b; // start --> 0^inf <s0 [r0]
		auto res=tm_step(tm.zero(),s0,-1);
		if(dbg1){
			merr<<"on_retL\n";
			prH2b(b);
		}
		if(res.is_back){// w1 s1> [r0]
			H3 c0(res.w1,res.s1,r0);
			H3s.add(c0);
			pre3L.add(c0);
			if(dbg1)prH3(c0);
		}else{// <s1 w1 [r0]
			retL.add(H2b(res.s1,push(res.w1,r0)));
			if(dbg1){
				pr_DFA_edge(push(res.w1,r0),{res.w1,r0});
				prH2b(H2b(res.s1,push(res.w1,r0)));
			}
		}
	}
	
	void on_DFA_edge(size_t r,DFA_edge e){
		pop_[r].emplace_back(e);
		for(auto s:r_s[r])on_H2_pop(H2(s,r),e);
	}
	
	void on_H2(H2 a){
		auto [s,r]=a; // s> [r]
		
		r_s[r].insert(s);
		for(auto e:pop_[r])on_H2_pop(a,e); // s> w [r0]
	}
	
	void on_H3(H3 a){
		auto [w,s,r]=a; // w s> [r]
		H2 a0(s,r);
		H2s.add(a0);
		pre23.add(a0,a);
	}
	
	void on_ret2(H2 a,H2b b){
		auto [s,r]=a; // s> [r]
		auto [s0,r0]=b; // <s0 [r0]
		for(auto c:pre23.at(a))on_H3_back(c,b); // w s> [r] --> w <s0 [r0] --> x |- c --> x
	}
	
	void on_ret3(H3 a,H2b b){
		auto [w,s,r]=a; // w s> [r]
		auto [s0,r0]=b; // <s0 [r0]
		for(auto a0:pre32.at(a))ret2.add(a0,b);
		for(auto a0:pre33.at(a))ret3.add(a0,b);
		if(pre3L.count(a))retL.add(b);
	}
	
	void on_pre23(H2 a,H3 c){
		for(auto b:ret2.at(a))on_H3_back(c,b);
	}
	
	void on_pre32(H3 a,H2 a0){
		for(auto b:ret3.at(a))ret2.add(a0,b);
	}
	
	void on_pre33(H3 a,H3 a0){
		for(auto b:ret3.at(a))ret3.add(a0,b);
	}
	
	void on_pre3L(H3 a){
		for(auto b:ret3.at(a))retL.add(b);
	}
	
	Decider(const TM_t &tm,int64_t maxT):tm(tm),maxT(maxT){}
	
	int64_t decide_(){
		::block_maxT=0;
		FAR::maxT=maxT;
		FAR::used_S=0;
		FAR::used_T=0;
		is_check=0;
		init();
		H3 c0(tm.zero(),0,1);
		H3s.add(c0);
		pre3L.add(c0);
		for(;;){
			if(!new_pops.empty()){
				auto [r,e]=new_pops.back();
				new_pops.pop_back();
				on_DFA_edge(r,e);
				continue;
			}
			if(H3s.on([this](H3 a){on_H3(a);}))continue;
			if(H2s.on([this](H2 a){on_H2(a);}))continue;
			if(retL.on([this](H2b a){on_retL(a);}))continue;
			if(pre3L.on([this](H3 a){on_pre3L(a);}))continue;
			if(ret2.on([this](H2 a,H2b b){on_ret2(a,b);}))continue;
			if(ret3.on([this](H3 a,H2b b){on_ret3(a,b);}))continue;
			if(pre23.on([this](H2 a,H3 b){on_pre23(a,b);}))continue;
			if(pre32.on([this](H3 a,H2 b){on_pre32(a,b);}))continue;
			if(pre33.on([this](H3 a,H3 b){on_pre33(a,b);}))continue;
			
			if(verbose){
				std::lock_guard _(mtx);
				std::cerr<<"decided:"
				<<" H2:"<<H2s.st.size()
				<<" H3:"<<H3s.st.size()
				<<" retL:"<<retL.st.size()
				<<" used_S:"<<used_S
				<<" used_T:"<<used_T
				<<" block_maxT:"<<block_maxT
				<<'\n';
			}
			//check();
			return 2;
		}
	}
	
	void pr_DFA_edge(size_t r,std::pair<Word,size_t> b){
		auto [w,r0]=b;
		merr<<w<<" ["<<r0<<"] = ["<<r<<"]\n";
	}
	void prH3(H3 a){
		auto [w,s,r]=a;
		merr<<w<<" "<<char('A'+s)<<"> ["<<r<<"]\n";
	}
	void prH2(H2 a){
		auto [s,r]=a;
		merr<<char('A'+s)<<"> ["<<r<<"]\n";
	}
	void prH2b(H2 a){
		auto [s,r]=a;
		merr<<"<"<<char('A'+s)<<" ["<<r<<"]\n";
	}
	void check(){
		is_check=1;
		#if 1
		H3 c0(tm.zero(),0,1);
		assert(H3s.count(c0));
		assert(pre3L.count(c0));
		assert(push(tm.zero(),1)==1);
		(H3s.on([this](H3 a){on_H3(a);}));
		(H2s.on([this](H2 a){on_H2(a);}));
		(retL.on([this](H2b a){on_retL(a);}));
		(pre3L.on([this](H3 a){on_pre3L(a);}));
		(ret2.on([this](H2 a,H2b b){on_ret2(a,b);}));
		(ret3.on([this](H3 a,H2b b){on_ret3(a,b);}));
		(pre23.on([this](H2 a,H3 b){on_pre23(a,b);}));
		(pre32.on([this](H3 a,H2 b){on_pre32(a,b);}));
		(pre33.on([this](H3 a,H3 b){on_pre33(a,b);}));
		assert(new_pops.empty());
		#endif
		if(dbg1){
			merr<<"DFA:\n";
			for(auto [k,r]:push_){
				pr_DFA_edge(r,k);
			}
			merr<<"\nH2s:\n";
			for(auto a:H2s.st){
				prH2(a);
				for(auto b:ret2.at(a)){
					merr<<"  ret: ";
					prH2b(b);
				}
				for(auto b:pre23.at(a)){
					merr<<"  pre23: ";
					prH3(b);
				}
			}
			merr<<"\nH3s:\n";
			for(auto a:H3s.st){
				prH3(a);
				if(pre3L.count(a))merr<<"  (pre3L)\n";
				for(auto b:ret3.at(a)){
					merr<<"  ret: ";
					prH2b(b);
				}
				for(auto b:pre33.at(a)){
					merr<<"  pre33: ";
					prH3(b);
				}
				for(auto b:pre32.at(a)){
					merr<<"  pre32: ";
					prH2(b);
				}
			}
			merr<<"\nretL:\n";
			for(auto b:retL.st){
				prH2b(b);
			}
		}
	}
	int64_t decide(){
		try{
			return decide_();
		}catch(WordLenError e){
			if(dbg1)merr<<"throw WordLenError\n";
			return 0;
		}catch(int64_t e){
			if(dbg1)merr<<"throw "<<e<<'\n';
			static thread_local std::mt19937 mt;
			if(verbose&&mt()%1000==0){
				std::lock_guard _(mtx);
				std::cerr<<"fail: "<<e
				<<" H2:"<<H2s.st.size()
				<<" H3:"<<H3s.st.size()
				<<" retL:"<<retL.st.size()
				<<" used_S:"<<used_S
				<<" used_T:"<<used_T
				<<" block_maxT:"<<block_maxT
				<<'\n';
			}
			return 0;
		}
	}
};
	#undef dbg1


};

/*
namespace FAR{
struct NFA{
	std::unordered_map<size_t,std::vector<std::pair<chr_t,size_t>>> pop_;
	std::map<std::pair<chr_t,size_t>,std::set<size_t>> push_;
	void push(size_t x,chr_t y,size_t z){
		if(push_[{y,x}].insert(z).second){
			pop_[z].emplace_back(y,x);
		}
	}
};
template<class DFA_state_t>
struct Decider{
	typedef std::tuple<size_t,size_t,int64_t> LRS;
	NFA lnfa,rnfa;
	IdAlloc<DFA_state_t> id;
	std::unordered_set<LRS> mset;
	std::vector<LRS> q;
	size_t rid(size_t lid,int64_t s){
		return 1+lid*N_STATE+s;
	}
	void mset_add(size_t lid,size_t rid,int64_t s){
		LRS w{lid,rid,s};
		if(mset.insert(w).second){
			q.push(w);
		}
	}
	int64_t decide(const TM &tm){
		size_t lid0=id.get_id(DFA_state_t());
		assert(lid0==0);
		lnfa.push(lid0,chr_t(),lid0);
		size_t rid0=0;
		rnfa.push(rid0,chr_t(),rid0);
		mset_add(lid0,rid0,0);
		for(size_t qi=0;qi<q.size();++qi){
			auto [lid,rid,s]=q[qi];
			auto st=rnfa.pop_[rid];
			for(auto [in,rid0]:st){
				auto &tr=tm.trans[s][in];
				if(tr.nxt==-1)return 0;
				if(tr.dir==1){
					
				}else{
					
				}
			}
		}
		return 0;
	}
};
}*/

template<class T>
std::vector<T> operator+(const std::vector<T> &a,const std::vector<T> &b){
	assert(a.size()==b.size());
	std::vector<T> c;
	c.reserve(a.size());
	for(size_t i=0;i<a.size();++i)c.emplace_back(a[i]+b[i]);
	return c;
}

template<class T>
std::vector<T> operator-(const std::vector<T> &a,const std::vector<T> &b){
	assert(a.size()==b.size());
	std::vector<T> c;
	c.reserve(a.size());
	for(size_t i=0;i<a.size();++i)c.emplace_back(a[i]-b[i]);
	return c;
}

template<class T>
std::vector<T> operator*(const std::vector<T> &a,T b){
	std::vector<T> c;
	c.reserve(a.size());
	for(size_t i=0;i<a.size();++i)c.emplace_back(a[i]*b);
	return c;
}

template<class T>
T dot(const std::vector<T> &a,const std::vector<T> &b){
	assert(a.size()==b.size());
	T c{};
	for(size_t i=0;i<a.size();++i)c+=a[i]*b[i];
	return c;
}

template<class T>
std::vector<T> unit(const std::vector<T> &a){
	return a*(1./(sqrt(dot(a,a))+1e-300));
}

template<class T>
std::vector<double> as_fvec(const std::vector<T> &a){
	std::vector<double> c;
	c.reserve(a.size());
	for(size_t i=0;i<a.size();++i)c.emplace_back(a[i]);
	return c;
}

std::optional<std::vector<int64_t>> as_ivec(const std::vector<double> &a){
	std::vector<int64_t> c;
	c.reserve(a.size());
	for(size_t i=0;i<a.size();++i){
		double ai=a[i];
		double r=round(a[i]);
		if(fabs(ai-r)>1e-3)return std::nullopt;
		c.emplace_back(int64_t(r));
	}
	return c;
}

struct Word_hash{
	size_t operator()(const Word &x)const{
		return x.hash();
	}
};

std::vector<TM> read_file(std::string fn){
	std::vector<TM> ret;
	std::cout<<"read from "<<fn<<'\n';
	std::ifstream ifs(fn);
	for(std::string ln;std::getline(ifs,ln);){
		if(ln.back()=='\r')ln.pop_back();
		if(ln.empty())continue;
		std::stringstream ss(ln);
		TM tm;
		ss>>tm;
		ret.emplace_back(tm);
	}
	std::cout<<"read "<<ret.size()<<" TMs\n";
	return ret;
}
std::vector<TM> read_file_1(std::string fn){
	std::vector<TM> ret;
	std::cout<<"read from "<<fn<<'\n';
	std::ifstream ifs(fn);
	for(std::string ln;std::getline(ifs,ln);){
		if(ln.back()=='\r')ln.pop_back();
		if(ln.empty())continue;
		std::stringstream ss(ln);
		int64_t id=0;
		TM tm;
		assert(ss>>id);
		ss>>tm;
		ret.emplace_back(tm);
	}
	std::cout<<"read "<<ret.size()<<" TMs\n";
	return ret;
}


struct TM_hash{
	size_t operator()(const TM&tm)const{
		size_t s=0;
		for(int64_t i=0;i<N_STATE;++i){
			for(int64_t j=0;j<N_CHAR;++j){
				const Trans &tr=tm.trans[i][j];
				s=s*12347+((tr.nxt+1)*N_CHAR+tr.out)*3+tr.dir;
			}
		}
		return s;
	}
};

template<class T>
std::vector<T> operator&(const std::vector<T> &a,const std::vector<T> &b){
	std::vector<T> c=a;
	for(const T &x:b)c.emplace_back(x);
	return c;
}



#define def_var(a,b) add_cmd(#a,[this]{read(b);})

struct LineExec{
	std::string ln;
	std::stringstream ss;
	TM tm;
	DH_Tape tape;
	bool fold_flag=0;
	size_t n=1,nl=0,nr=0,maxT=1,maxS=1e8;
	int64_t result=0;
	size_t sim_maxT=0;
	std::map<std::string,std::function<void()>> cmd_mp;
	void add_cmd(std::string cmd,std::function<void()> f){
		cmd_mp[cmd]=f;
	}
	std::shared_ptr<TM_t> get_tm(){
		if(fold_flag)return std::make_shared<FoldTapeTM>(tm,nl,nr);
		return std::make_shared<BlockTM>(tm,n);
	}
	inline static int64_t id=1;
	LineExec(std::string ln_):ln(ln_),ss(ln){
		if(!(ss>>tm))assert(0);
		//std::cerr<<'\n'<<ln<<'\n';
		
		ChrTransform::init();
		def_var(chr_LRUH,ChrTransform::len_h);
		def_var(chr_H,ChrTransform::len_h_no_lru);
		def_var(chr_LRUn,ChrTransform::lru_n);
		def_var(chr_asth,ChrTransform::add_state_to_h);
		def_var(chr_max_bsz,ChrTransform::max_bsz);
		def_var(chr_mnc,ChrTransform::mnc);
		def_var(chr_mod,ChrTransform::mod);
		
		add_cmd("MitM_CTL",[this]{
			def_var(n,n);
			def_var(maxT,maxT);
			add_cmd("fold",[this]{
				assert(nl==0&&nr==0&&!fold_flag);
				read(nl);
				read(nr);
				assert(nl&&nr);
				fold_flag=1;
			});
			add_cmd("sim",[this]{
				assert(!sim_maxT);
				read(sim_maxT);
				tape.check_halt(tm,sim_maxT);
				if(tape.s==-1)throw (int64_t)0;
			});
			
			using namespace MitM_CTL;
			add_cmd("LRUpair",[this]{
				LRUpair::init();
				def_var(LRUH,LRUpair::len_h);
				def_var(H,LRUpair::len_h_no_lru);
				def_var(tH,LRUpair::len_h_tail);
				add_cmd("run",[this]{
					MitM_CTL::Decider<LRUpair> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});
			
			add_cmd("set_pair",[this]{
				set_pair::init();
				def_var(setH,set_pair::len_h);
				def_var(H,set_pair::len_h_no_lru);
				def_var(tH,set_pair::len_h_tail);
				add_cmd("run",[this]{
					MitM_CTL::Decider<set_pair> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("NGset",[this]{
				NGset::init();
				def_var(NG_n,NGset::NG_n);
				def_var(H,NGset::len_h);
				add_cmd("run",[this]{
					MitM_CTL::Decider<NGset> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("NG",[this]{
				NG::init();
				def_var(NG_n,NG::NG_n);
				def_var(tH,NG::tH);
				def_var(pos_mod,NG::pos_mod);
				add_cmd("run",[this]{
					MitM_CTL::Decider<NG> f;
					auto tm0=get_tm();
					if(to_coq){
					if(n==1&&!fold_flag&&NG::pos_mod==1&&NG::tH==0&&ChrTransform::max_bsz==0){
						std::lock_guard _(mtx);
						assert(ChrTransform::len_h>=ChrTransform::len_h_no_lru);
						assert(ChrTransform::lru_n>=1);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (NG "<<sim_maxT<<" "<<maxT<<" "<<maxT<<" "<<NG::NG_n<<" "
						<<ChrTransform::len_h_no_lru<<" "<<(ChrTransform::len_h-ChrTransform::len_h_no_lru)<<" "<<(ChrTransform::lru_n-1)
						<<" "<<(ChrTransform::add_state_to_h?"true":"false")
						<<"). ";
						merr<<"Time Qed.\n\n";
						merr.flush();
						//NG(simT maxT maxS NG_n len1 len2 LRU_n:N)(asth:bool)
					}
					throw (int64_t)2;
					}
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			/*add_cmd("Test",[this]{
				Test::init();
				add_cmd("run",[this]{
					MitM_CTL::Decider<Test> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});*/

			add_cmd("NestRep",[this]{
				NestRep::init();
				def_var(H,NestRep::H);
				def_var(tH,NestRep::tH);
				def_var(dep,NestRep::dep);
				def_var(mnc,NestRep::mnc);
				def_var(mod,NestRep::mod);
				add_cmd("run",[this]{
					MitM_CTL::Decider<NestRep> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("RWL_mod",[this]{
				RWL_mod::init();
				def_var(H,RWL_mod::len_h);
				def_var(tH,RWL_mod::len_h_tail);
				def_var(mnc,RWL_mod::mnc);
				def_var(mod,RWL_mod::mod);
				add_cmd("run",[this]{
					MitM_CTL::Decider<RWL_mod> f;
					auto tm0=get_tm();
					assert(RWL_mod::len_h>=RWL_mod::len_h_tail);
					
					if(to_coq&&ChrTransform::len_h==0){
						std::lock_guard _(mtx);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (RWL_mod "<<sim_maxT<<" "<<maxT<<" "<<maxT<<" "<<n<<" "<<_block_maxT<<" "
						<<RWL_mod::mnc<<" "<<RWL_mod::mod
						<<" "<<(RWL_mod::len_h-RWL_mod::len_h_tail)<<" "<<RWL_mod::len_h_tail<<"). ";
						merr<<"Time Qed.\n\n";
						merr.flush();
						throw (int64_t)2;
					}
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("RWS_mod",[this]{
				RWS_mod::init();
				def_var(H,RWS_mod::len_h);
				def_var(mnc,RWS_mod::mnc);
				def_var(mod,RWS_mod::mod);
				add_cmd("run",[this]{
					MitM_CTL::Decider<RWS_mod> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("RNGS_mod",[this]{
				RNGS_mod::init();
				def_var(H,RNGS_mod::len_h);
				def_var(mnc,RNGS_mod::mnc);
				def_var(mod,RNGS_mod::mod);
				def_var(NG_n,RNGS_mod::NG_n);
				def_var(bs_n,RNGS_mod::bs_n);
				add_cmd("run",[this]{
					MitM_CTL::Decider<RNGS_mod> f;
					auto tm0=get_tm();
					if(to_coq&&ChrTransform::len_h==0){
						std::lock_guard _(mtx);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (RNGS_mod_QSym "<<sim_maxT<<" "<<maxT<<" "<<maxT<<" 0 0 0 "
						<<RNGS_mod::mnc<<" "<<RNGS_mod::mod<<" "<<RNGS_mod::NG_n<<" "<<RNGS_mod::len_h<<" "<<RNGS_mod::bs_n
						<<"). ";
						merr<<"Time Qed.\n\n";
						//(simT maxT maxS len1 len2 LRU_n mnc mod_ NG_n len_h)
						merr.flush();
						throw (int64_t)2;
					}
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("RS_mod",[this]{
				RS_mod::init();
				def_var(H,RS_mod::len_h);
				def_var(mnc,RS_mod::mnc);
				def_var(mod,RS_mod::mod);
				def_var(NG_n,RS_mod::NG_n);
				add_cmd("run",[this]{
					MitM_CTL::Decider<RS_mod> f;
					auto tm0=get_tm();
					if(to_coq){
						std::lock_guard _(mtx);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (RNGS_mod_QSym "<<sim_maxT<<" "<<maxT<<" "<<maxT
						<<" "<<ChrTransform::len_h_no_lru<<" "<<(ChrTransform::len_h-ChrTransform::len_h_no_lru)<<" "<<(ChrTransform::lru_n-1)
						<<" "<<RS_mod::mnc<<" "<<RS_mod::mod<<" "<<1<<" "<<RS_mod::len_h<<" "<<RS_mod::NG_n
						<<"). ";
						merr<<"Time Qed.\n\n";
						//(simT maxT maxS len1 len2 LRU_n mnc mod_ NG_n len_h)
						merr.flush();
						throw (int64_t)2;
					}
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("RndDFA",[this]{
				RndDFA::init();
				def_var(H,RndDFA::len_h);
				def_var(mnc,RndDFA::mnc);
				def_var(mod,RndDFA::mod);
				def_var(seed,RndDFA::seed);
				add_cmd("run",[this]{
					MitM_CTL::Decider<RndDFA> f;
					RndDFA::init2();
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("ExRWL",[this]{
				ExRWL::init();
				def_var(H,ExRWL::len_h);
				def_var(tH,ExRWL::len_h_tail);
				def_var(bsz,ExRWL::block_size);
				add_cmd("run",[this]{
					MitM_CTL::Decider<ExRWL> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("CPS_LRU",[this]{
				CPS_LRU::init();
				def_var(LRUH,CPS_LRU::len_h);
				def_var(H,CPS_LRU::len_h_no_lru);
				def_var(tH,CPS_LRU::len_h_tail);
				add_cmd("run",[this]{
					MitM_CTL::Decider<CPS_LRU> f;
					auto tm0=get_tm();
					if(to_coq&&ChrTransform::len_h==0){
						std::cerr<<tm<<'\n';
						std::lock_guard _(mtx);
						assert(CPS_LRU::len_h>=CPS_LRU::len_h_no_lru+CPS_LRU::len_h_tail);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (CPS_LRU "<<sim_maxT<<" "<<maxT<<" "<<maxT<<" "<<n<<" "<<_block_maxT
						<<" "<<CPS_LRU::len_h_no_lru<<" "<<(CPS_LRU::len_h-CPS_LRU::len_h_no_lru-CPS_LRU::len_h_tail)<<" "<<CPS_LRU::len_h_tail<<" 0"<<"). ";
						merr<<"Time Qed.\n\n";
						merr.flush();
						throw (int64_t)2;
					}
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});

			add_cmd("CPS_LRU_BD",[this]{
				CPS_LRU_BD::init();
				def_var(LRUH,CPS_LRU_BD::len_h);
				def_var(H,CPS_LRU_BD::len_h_no_lru);
				add_cmd("run",[this]{
					MitM_CTL::Decider<CPS_LRU_BD> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});
			
			add_cmd("unique_wall",[this]{
				unique_wall::init();
				def_var(setH,unique_wall::len_h);
				def_var(H,unique_wall::len_h_no_lru);
				def_var(tH,unique_wall::len_h_tail);
				def_var(mnc,unique_wall::mnc);
				def_var(sorted,unique_wall::sorted);
				add_cmd("run",[this]{
					MitM_CTL::Decider<unique_wall> f;
					auto tm0=get_tm();
					throw f.decide(*tm0,maxT,tm0->from(tape));
				});
			});
		});
		
		add_cmd("FAR",[this]{
			def_var(n,n);
			def_var(maxT,maxT);
			
			using namespace MitM_CTL;
			
			add_cmd("RWL_mod",[this]{
				RWL_mod::init();
				def_var(H,RWL_mod::len_h);
				def_var(tH,RWL_mod::len_h_tail);
				def_var(mnc,RWL_mod::mnc);
				def_var(mod,RWL_mod::mod);
				add_cmd("run",[this]{
					assert(RWL_mod::len_h>=RWL_mod::len_h_tail);
					if(to_coq&&ChrTransform::len_h==0&&sim_maxT==0){
						std::lock_guard _(mtx);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (RWL_mod_FAR "<<maxT*2<<" "<<maxT<<" "<<n<<" "<<_block_maxT<<" "
						<<RWL_mod::mnc<<" "<<RWL_mod::mod
						<<" "<<(RWL_mod::len_h-RWL_mod::len_h_tail)<<" "<<RWL_mod::len_h_tail<<"). ";
						merr<<"Time Qed.\n\n";
						merr.flush();
						throw (int64_t)2;
					}
					auto tm0=get_tm();
					FAR::Decider<RWL_mod> f(*tm0,maxT);
					throw f.decide();
				});
			});

			add_cmd("CPS_LRU",[this]{
				CPS_LRU::init();
				def_var(LRUH,CPS_LRU::len_h);
				def_var(H,CPS_LRU::len_h_no_lru);
				def_var(tH,CPS_LRU::len_h_tail);
				add_cmd("run",[this]{
					if(to_coq&&ChrTransform::len_h==0&&sim_maxT==0){
						std::cerr<<tm<<'\n';
						std::lock_guard _(mtx);
						assert(CPS_LRU::len_h>=CPS_LRU::len_h_no_lru+CPS_LRU::len_h_tail);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (CPS_LRU_FAR "<<maxT*2<<" "<<maxT<<" "<<n<<" "<<_block_maxT
						<<" "<<CPS_LRU::len_h_no_lru<<" "<<(CPS_LRU::len_h-CPS_LRU::len_h_no_lru-CPS_LRU::len_h_tail)<<" "<<CPS_LRU::len_h_tail<<" 0"<<"). ";
						merr<<"Time Qed.\n\n";
						merr.flush();
						throw (int64_t)2;
					}
					auto tm0=get_tm();
					FAR::Decider<CPS_LRU> f(*tm0,maxT);
					throw f.decide();
				});
			});

			add_cmd("RNGS_mod",[this]{
				RNGS_mod::init();
				def_var(H,RNGS_mod::len_h);
				def_var(mnc,RNGS_mod::mnc);
				def_var(mod,RNGS_mod::mod);
				def_var(NG_n,RNGS_mod::NG_n);
				def_var(bs_n,RNGS_mod::bs_n);
				add_cmd("run",[this]{
					if(to_coq&&ChrTransform::len_h==0){
						std::lock_guard _(mtx);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (RNGS_mod_QSym_FAR "<<maxT*2<<" "<<maxT<<" 0 0 0 "
						<<RNGS_mod::mnc<<" "<<RNGS_mod::mod<<" "<<RNGS_mod::NG_n<<" "<<RNGS_mod::len_h<<" "<<RNGS_mod::bs_n
						<<"). ";
						merr<<"Time Qed.\n\n";
						//(simT maxT maxS len1 len2 LRU_n mnc mod_ NG_n len_h)
						merr.flush();
						throw (int64_t)2;
					}
					auto tm0=get_tm();
					FAR::Decider<RNGS_mod> f(*tm0,maxT);
					throw f.decide();
				});
			});

			add_cmd("RS_mod",[this]{
				RS_mod::init();
				def_var(H,RS_mod::len_h);
				def_var(mnc,RS_mod::mnc);
				def_var(mod,RS_mod::mod);
				def_var(NG_n,RS_mod::NG_n);
				add_cmd("run",[this]{
					if(to_coq){
						std::lock_guard _(mtx);
						merr<<"Lemma nonhalt"<<(id++)<<": ~halts (TM_from_str ";
						merr<<'"'<<tm<<'"';
						merr<<") c0.\n";
						merr<<"Proof. ";
						merr<<"solve_cert (RNGS_mod_QSym_FAR "<<maxT*2<<" "<<maxT
						<<" "<<ChrTransform::len_h_no_lru<<" "<<(ChrTransform::len_h-ChrTransform::len_h_no_lru)<<" "<<(ChrTransform::lru_n-1)
						<<" "<<RS_mod::mnc<<" "<<RS_mod::mod<<" "<<1<<" "<<RS_mod::len_h<<" "<<RS_mod::NG_n
						<<"). ";
						merr<<"Time Qed.\n\n";
						//(simT maxT maxS len1 len2 LRU_n mnc mod_ NG_n len_h)
						merr.flush();
						throw (int64_t)2;
					}
					auto tm0=get_tm();
					FAR::Decider<RS_mod> f(*tm0,maxT);
					throw f.decide();
				});
			});

			add_cmd("NG",[this]{
				NG::init();
				def_var(NG_n,NG::NG_n);
				def_var(tH,NG::tH);
				def_var(pos_mod,NG::pos_mod);
				add_cmd("run",[this]{
					auto tm0=get_tm();
					FAR::Decider<NG> f(*tm0,maxT);
					throw f.decide();
				});
			});

			add_cmd("NG1",[this]{
				NG1::init();
				def_var(NG_n,NG1::NG_n);
				add_cmd("run",[this]{
					auto tm0=get_tm();
					FAR::Decider<NG1> f(*tm0,maxT);
					throw f.decide();
				});
			});
		});
		result=exec();
	}
	std::string next(){
		std::string s;
		read(s);
		return s;
	}
	template<class T>
	void read(T &x){
		if(!(ss>>x))assert(0);
	}
	int64_t exec(){
		try{
			for(;;){
				std::string s=next();
				auto it=cmd_mp.find(s);
				if(it==cmd_mp.end()){
					std::cerr<<"unknown cmd: "<<s<<'\n';
					assert(0);
				}
				it->second();
			}
		}catch(int64_t result){
			return result;
		}
	}
};
#undef def_var

bool app_stopped = false;

std::vector<std::string> todo;
int main_verify(int argc,char **argv,bool may_fail,bool dbg_on=0){
	if(argc<2)assert(0);
	std::ifstream ifs(argv[1]);
	assert(ifs.is_open());
	for(std::string ln;std::getline(ifs,ln);){
		while(isspace(ln.back()))ln.pop_back();
		if(ln.empty())continue;
		todo.emplace_back(ln);
	}
	auto t0=std::chrono::high_resolution_clock::now();
	size_t n_decided=0,n_tot=0;
	auto show_status=[&](){
		auto t1=std::chrono::high_resolution_clock::now();
		auto dt=std::chrono::duration_cast<std::chrono::seconds>(t1-t0).count();
		long eta=(todo.size()*1./(1+n_tot))*dt;
		std::cout<<n_decided<<"/"<<n_tot<<"    "<<dt<<"s/"<<eta<<"s"<<"    "<<n_decided*1./n_tot<<"         \r";
		std::cout.flush();
	};
	size_t sz=todo.size();
	//#pragma omp parallel for schedule(dynamic) num_threads(8)
	for(size_t i=0;i<sz;++i){
		{
			std::lock_guard _(mtx);
			++n_tot;
		}
		if(app_stopped)assert(0);
		if(dbg_on)dbg=1;
		LineExec exec(todo[i]);
		if(exec.result==2){
			std::lock_guard _(mtx);
			++n_decided;
			show_status();
			//merr<<exec.tm<<" N\n";
		}else if(exec.result==1){
			std::lock_guard _(mtx);
			++n_decided;
			show_status();
			//merr<<exec.tm<<" H\n";
		}else{
			std::lock_guard _(mtx);
			std::cerr<<"failed:\n";
			std::cerr<<todo[i]<<'\n';
			if(!may_fail){
				dbg=1;
				LineExec exec1(todo[i]);
				merr.flush();
				assert(0);
			}
		}
	}
	std::cout<<'\n';
	
	return 0;
}

thread_local std::mt19937 mt;
double rnd_gaussian(){
	return std::normal_distribution{0.0,1.0}(mt);
}

struct Action{
	std::string s;
	int64_t used_T=0;
	size_t n_solved=0,n_visited=0;
	double value=0;
	void upd_value(size_t n_tot,int64_t t_tot,double c=1){
		if(n_visited>=n_tot)value=-1;
		else{
			double p=(n_solved+c)*1./(used_T+2);
			value=std::max(0.,p+rnd_gaussian()*sqrt(p*(1-p)/(used_T+2)));
		}
	}
	friend bool operator<(const Action &a,const Action &b){
		return a.value>b.value;
	}
	friend std::ostream &operator<<(std::ostream &os,const Action &x){
		os<<"Action "<<x.used_T<<" "<<x.n_solved<<" "<<x.n_visited<<'\n';
		os<<x.s<<'\n';
		return os;
	}
	friend std::istream &operator>>(std::istream &is,Action &x){
		std::string s;
		if(!(is>>s))return is;
		assert(s=="Action");
		if(!(is>>x.used_T>>x.n_solved>>x.n_visited))assert(0);
		std::getline(is,s);
		while(!s.empty()&&isspace(s.back()))s.pop_back();
		assert(s.empty());
		std::getline(is,s);
		while(!s.empty()&&isspace(s.back()))s.pop_back();
		assert(!s.empty());
		x.s=s;
		return is;
	}
};

constexpr int64_t T_ckpt=1000;
constexpr bool no_explore=0;

struct UCB{
	std::vector<std::pair<TM,bool>> tms;
	std::vector<Action> actions;
	size_t n_solved=0,n_visited=0;
	void init(const std::vector<TM> &ls){
		for(TM tm:ls)tms.emplace_back(tm,0);
	}
	void ignore(const std::vector<TM> &ls){
		std::unordered_set<TM,TM_hash> ls0(ls.begin(),ls.end());
		for(auto &[tm,flag]:tms)if(!flag&&ls0.count(tm)){
			flag=1;
			//++n_solved;
		}
	}
	void load(){
		std::lock_guard _(mtx);
		std::ifstream ifs("ucb.txt");
		assert(ifs.is_open());
		actions.clear();
		
		for(;;){
			Action a;
			if(!(ifs>>a))break;
			actions.emplace_back(a);
		}
		assert(!actions.empty());
	}
	void upd_value(){
		double c=mt()%2?1:0.01;//pow(2,-(mt()%10));
		auto t_tot=get_t_tot();
		for(Action &a:actions)a.upd_value(tms.size(),t_tot,c);
	}
	void store(){
		std::lock_guard _(mtx);
		upd_value();
		std::stable_sort(actions.begin(),actions.end());
		std::ofstream ofs("ucb.txt");
		assert(ofs.is_open());
		for(auto &a:actions)ofs<<a;
	}
	int64_t get_t_tot(){
		int64_t s=2;
		for(Action &a:actions)s+=a.used_T;
		return s;
	}
	bool finished(){
		for(Action &a:actions)if(a.n_visited<tms.size())return 0;
		return 1;
	}
	std::optional<std::pair<size_t,size_t>> select_op(){
		std::lock_guard _(mtx);
		upd_value();
		size_t p=0,sz=actions.size();
		for(size_t i=0;i<sz;++i){
			if(actions[i].value>actions[p].value)p=i;
		}
		Action &a=actions[p];
		while(a.n_visited<tms.size()&&tms[a.n_visited].second)++a.n_visited;
		if(a.n_visited<tms.size()){
			size_t i=mt()%tms.size();
			if(no_explore||mt()%3||i<a.n_visited){
				i=a.n_visited++;
			}else{
				while(i<tms.size()&&tms[i].second)++i;
				if(i>=tms.size())return std::nullopt;
			}
			return std::make_pair(p,i);
		}
		return std::nullopt;
	}
	void solve(size_t p,size_t i){
		Action &a=actions.at(p);
		std::stringstream ss;
		ss<<tms.at(i).first<<' '<<a.s;
		//std::cerr<<'\n'<<ss.str()<<'\n';
		auto t0=std::chrono::high_resolution_clock::now();
		auto get_used_T=[t0](){
			auto t1=std::chrono::high_resolution_clock::now();
			auto dt=std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
			return dt+1;
		};
		LineExec exec(ss.str());
		std::lock_guard _(mtx);
		n_visited+=1;
		//std::cout<<exec.result<<" "<<ss.str()<<'\n';
		a.used_T+=get_used_T();
		if(exec.result==1||exec.result==2){
			if(tms.at(i).second)return;
			tms.at(i).second=1;
			merr<<ss.str()<<'\n';
			a.n_solved+=1;
			n_solved+=1;
			std::cout<<"\r"<<n_solved<<'/'<<n_visited<<"    "<<get_t_tot()/1000000<<"s    ";
			std::cout.flush();
		}else if(n_visited%T_ckpt==0){
			std::cout<<"\r"<<n_solved<<'/'<<n_visited<<"    "<<get_t_tot()/1000000<<"s    ";
			std::cout.flush();
		}
	}
};

void sigint_handler(int sig){
	if(sig == SIGINT){
		std::cerr << "\nctrl+c pressed!" << std::endl;
		app_stopped = true;
	}
}

int main_ucb(int argc,char **argv){
	//auto ls=read_file("../../BB25/3x3.todo.txt");
	//auto ls=read_file("../BB25/2x5_holdouts_217.txt");
	//auto ls=read_file("../BB25/BB6_unknown.txt");
	//auto ls=read_file("../BB25/BB6_holdouts_12444.txt");
	auto ls=read_file("ucb_in.txt");
	//auto ls=read_file("../BB25/BB6_holdouts_15535.txt");
	std::mt19937 mt(123);
	std::random_shuffle(ls.begin(),ls.end(),[&](size_t n){return mt()%n;});
	UCB ucb;
	ucb.init(ls);
	ucb.load();
	//ucb.ignore(read_file("ucb_2x5_log.txt"));
	ucb.ignore(read_file("ucb_log.txt"));
	for(;;){
		#pragma omp parallel for schedule(dynamic) num_threads(8)
		for(size_t i=0;i<T_ckpt;++i){
			if(app_stopped)continue;
			auto op=ucb.select_op();
			if(!op)continue;
			ucb.solve(op->first,op->second);
		}
		merr.flush();
		ucb.store();
		if(ucb.finished()){
			std::cerr << "\nfinished" << std::endl;
			break;
		}
		if(app_stopped)break;
	}
	return 0;
}

std::unordered_set<TM,TM_hash> read_file_sc(const char *fn){
	std::unordered_set<TM,TM_hash> ret;
	std::cout<<"read from "<<fn<<'\n';
	std::ifstream ifs(fn);
	size_t i=0;
	for(std::string ln;std::getline(ifs,ln);++i){
		if(ln.back()=='\r')ln.pop_back();
		if(ln.empty())continue;
		if(i%3)continue;
		std::stringstream ss(ln);
		TM tm;
		ss>>tm;
		ret.insert(tm);
	}
	std::cout<<"read "<<ret.size()<<" TMs\n";
	return ret;
}

int main_0(){
	auto t0=std::chrono::high_resolution_clock::now();
	auto ls=read_file("in0.txt");//dbg=1;
	//auto ls=read_file_1("ind/BB6_halt_3e10.txt");
	//auto ls=read_file_1("ind/ind_srec_1e5.txt");
	//auto ls=read_file("../visualize/unclassified_2.txt");
	//auto ls=read_file("../visualize/input.txt");
	//auto ls=read_file_1("BB6_TC_1e10_S1e8.txt");
	//auto ls=read_file("../BB25/BB6_holdouts_7071.txt");
	//auto ls=read_file("./ind/BB6_holdouts_7234_sorted.txt");
	//auto ls1=read_file("srec_gt_1e10.txt");
	std::unordered_set<TM,TM_hash> ls0;//(ls1.begin(),ls1.end());
	//ls0.clear();
	//auto ls0=read_file_sc("MITMWFAR_certs.txt");
	size_t sz=ls.size(),n_ignore=0,n_finished=0,n_decided=0;
	//MitMWFAR::WDFA_Gen<MitMWFAR::NG> f;
	//f.gen();
	#pragma omp parallel for schedule(dynamic) num_threads(1)
	for(size_t i=0;i<sz;++i){
		if(app_stopped){
			std::lock_guard _(mtx);
			exit(0);
			continue;
		}
		auto show_status=[&](){
			auto t1=std::chrono::high_resolution_clock::now();
			auto dt=std::chrono::duration_cast<std::chrono::seconds>(t1-t0).count();
			long eta=(sz*1./(1+n_finished))*dt;
			std::cout<<n_decided<<"/"<<n_finished<<"/"<<sz<<"    "<<dt<<"s/"<<eta<<"s"<<"    "<<n_decided*1./n_finished<<"         \r";
			std::cout.flush();
		};
		TM tm=ls.at(i);
		bool ig=ls0.count(tm);
		if(!ig){
			//f.output(tm);
		}
		std::lock_guard _(mtx);
		n_ignore+=ig;
		++n_finished;
		show_status();
	}
	std::cout<<'\n';
	return 0;
}

int main(int argc,char **argv){
	if(argc==1)return main_0();
	assert(argc>=2);
	if(!strcmp(argv[1],"ucb")){
		signal(SIGINT,sigint_handler);
		main_ucb(argc-1,argv+1);
	}else if(!strcmp(argv[1],"verify")){
		main_verify(argc-1,argv+1,0);
	}else if(!strcmp(argv[1],"dbg")){
		main_verify(argc-1,argv+1,1,1);
	}else if(!strcmp(argv[1],"exec")){
		main_verify(argc-1,argv+1,1);
	}else if(!strcmp(argv[1],"to_coq")){
		assert(argc>=3);
		to_coq=1;
		LineExec::id=atoi(argv[2]);
		main_verify(argc-2,argv+2,1);
	}else{
		std::cerr<<"unknown task: "<<argv[1]<<'\n';
		assert(0);
	}
	return 0;
}
