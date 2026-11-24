#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>
#include <deque>
#include <cstring>
#include <mutex>
#include <csignal>
#include <optional>
#include <functional>

std::ofstream log_stream("log.txt");
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
#undef assert
#define assert(x) ((x)||assert_fail(#x,__LINE__))
[[noreturn]] bool assert_fail(const char *info,int line){
	merr.flush();
	std::cerr<<"line: "<<line<<", assertion failed: "<<info<<'\n';
	exit(1);
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
bool dbg=0;

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

#define BB7

#ifdef BB64
constexpr int64_t N_STATE=60;
constexpr int64_t N_CHAR=2;
constexpr int64_t log2_N_STATE=6;
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

constexpr int64_t tm_fmt=2;
struct Trans{
	int8_t nxt,out,dir,_pad=0;
	Trans():nxt(-1),out(1),dir(1){}
	Trans(int64_t s,int64_t o,int64_t d):nxt((int8_t)s),out((int8_t)o),dir((int8_t)d){}
	bool operator==(const Trans &tr)const{
		return nxt==tr.nxt && out==tr.out && dir==tr.dir;
	}
	bool operator<(const Trans &tr)const{
		return
		nxt!=tr.nxt ? nxt<tr.nxt:
		out!=tr.out ? out<tr.out:
		dir<tr.dir;
	}
	friend std::ostream &operator<<(std::ostream &os,const Trans &tr){
		os<<("_1"[tr.out])<<' '<<(tr.dir==1?'r':'l')<<' ';
		if(tr.nxt==-1)os<<"halt";
		else os<<(int)tr.nxt;
		return os;
	}
};

struct TM{
	Trans trans[N_STATE][N_CHAR];
	TM(){}
	bool operator==(const TM &tm)const{
		for(int64_t i=0;i<N_STATE;++i)
		for(int64_t j=0;j<N_CHAR;++j){
			if(!(trans[i][j]==tm.trans[i][j]))return 0;
		}
		return 1;
	}
	bool operator<(const TM &tm)const{
		for(int64_t i=0;i<N_STATE;++i)
		for(int64_t j=0;j<N_CHAR;++j){
			if(!(trans[i][j]==tm.trans[i][j]))return trans[i][j]<tm.trans[i][j];
		}
		return 0;
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
		if(tm_fmt==1)
		for(int64_t s=0;s<N_STATE;++s){
			for(int64_t i=0;i<2;++i){
				os<<s<<' '<<("_1"[i])<<"  "<<tm.trans[s][i]<<'\n';
			}
		}
		if(tm_fmt==2)
		for(int64_t s=0;s<N_STATE;++s){
			if(s)os<<'_';
			for(int64_t i=0;i<N_CHAR;++i){
				const Trans &tr=tm.trans[s][i];
				if(tr.nxt==-1)os<<"---";
				else os<<char('0'+tr.out)<<(tr.dir==1?'R':tr.dir==-1?'L':'M')<<char('A'+tr.nxt);
			}
		}
		return os;
	}
	friend std::istream &operator>>(std::istream &is,TM &tm){
		std::string str;
		assert(is>>str);
		if(str.length()!=N_STATE*N_CHAR*3+N_STATE-1){
			std::cerr<<"\""<<str<<"\"\n";
		}
		assert(str.length()==N_STATE*N_CHAR*3+N_STATE-1);
		int64_t p=0;
		for(int64_t i=0;i<N_STATE;++i){
			if(i){
				assert(str[p]=='_');
				++p;
			}
			for(int64_t j=0;j<N_CHAR;++j){
				Trans &tr=tm.trans[i][j];
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
constexpr int64_t MAX_WORD_LENGTH=64*2;
struct Word{
	uint64_t lo,hi;
	int64_t len;
	Word():lo(0),hi(0),len(0){}
	Word(unsigned __int128 d,int64_t n):lo((uint64_t)d),hi((uint64_t)(d>>64)),len(n){
		assert(0<=len&&len<=MAX_WORD_LENGTH);
	}
	Word(uint64_t l,uint64_t h,int64_t n):lo(l),hi(h),len(n){
		assert(0<=len&&len<=MAX_WORD_LENGTH);
	}
	inline unsigned __int128 to_u128()const{
		return ((unsigned __int128)hi<<64)|lo;
	}
	inline Word operator+(const Word &w)const{
		if(len+w.len>MAX_WORD_LENGTH)throw WordLenError();
		if(w.len==0)return *this;
		if(len==0)return w;
		return Word(to_u128()|(w.to_u128()<<len),len+w.len);
	}
	inline Word set(int64_t x,int64_t y)const{
		assert(0<=x&&x<len);
		assert(0<=y&&y<2);
		Word r=*this;
		if(x<64){
			if(y)r.lo|=(1ULL<<x);
			else r.lo&=~(1ULL<<x);
		}else{
			if(y)r.hi|=(1ULL<<(x-64));
			else r.hi&=~(1ULL<<(x-64));
		}
		return r;
	}
	inline Word pop()const{return Word(to_u128()>>1,len-1);}
	inline std::pair<Word,Word> pop(int64_t n)const{
		assert(n<MAX_WORD_LENGTH);
		unsigned __int128 d=to_u128();
		unsigned __int128 mask=(n==0)?0:(((unsigned __int128)1<<n)-1);
		return {Word(d&mask,n),Word(d>>n,std::max<int64_t>(0L,len-n))};
	}
	inline uint8_t at(int64_t x)const{
		assert(0<=x&&x<len);
		return (x<64)?((lo>>x)&1):((hi>>(x-64))&1);
	}
	inline bool operator==(const Word &w)const{
		return lo==w.lo&&hi==w.hi&&len==w.len;
	}
	inline bool operator!=(const Word &w)const{
		return !(*this==w);
	}
	inline bool operator<(const Word &w)const{
		return len!=w.len?len<w.len:(hi!=w.hi?hi<w.hi:lo<w.lo);
	}
	friend std::ostream &operator<<(std::ostream &os,Word w){
		for(int64_t i=0;i<w.len;++i)os<<(int64_t)w.at(i);
		return os;
	}
	inline size_t hash()const{
		uint64_t h=lo^(hi*0x9e3779b97f4a7c15ULL)^((uint64_t)len*0xbf58476d1ce4e5b9ULL);
		h^=h>>30;
		h*=0xbf58476d1ce4e5b9ULL;
		h^=h>>27;
		return (size_t)h;
	}
	inline int64_t size()const{
		return len;
	}
	inline bool operator!()const{
		return (lo|hi)==0;
	}
	inline uint64_t to_u64()const{return lo;}
	
	inline chr_t to_chr()const{
		assert(len==log2_N_CHAR);
		return (chr_t)lo;
	}
	inline Word at(int64_t x,int64_t n)const{
		int64_t shift=x*n;
		unsigned __int128 d=(shift>=MAX_WORD_LENGTH)?0:(to_u128()>>shift);
		unsigned __int128 mask=(n==0)?0:(((unsigned __int128)1<<n)-1);
		return Word(d&mask,n);
	}
	inline Word del(int64_t x,int64_t n)const{
		int64_t p=x*n;
		unsigned __int128 d=to_u128();
		unsigned __int128 lo_mask=(p==0)?0:(((unsigned __int128)1<<p)-1);
		unsigned __int128 low=d&lo_mask;
		unsigned __int128 high=(p+n>=MAX_WORD_LENGTH)?0:((d>>(p+n))<<p);
		return Word(low|high,p+std::max<int64_t>(0L,len-p-n));
	}
	inline Word set_chr(int64_t x,chr_t y)const{
		if constexpr(log2_N_CHAR==1){
			Word r=*this;
			if(x<64){
				if(y)r.lo|=(1ULL<<x);
				else r.lo&=~(1ULL<<x);
			}else{
				if(y)r.hi|=(1ULL<<(x-64));
				else r.hi&=~(1ULL<<(x-64));
			}
			return r;
		}else{
			int64_t shift=x*log2_N_CHAR;
			unsigned __int128 mask=(((unsigned __int128)1<<log2_N_CHAR)-1)<<shift;
			unsigned __int128 d=(to_u128()&~mask)|((unsigned __int128)(uint64_t)y<<shift);
			return Word(d,len);
		}
	}
	inline chr_t get_chr(int64_t x)const{
		if constexpr(log2_N_CHAR==1){
			return (x<64)?((lo>>x)&1):((hi>>(x-64))&1);
		}else{
			return (chr_t)((to_u128()>>(x*log2_N_CHAR))&((1u<<log2_N_CHAR)-1));
		}
	}
	inline std::pair<Word,Word> pop_chrs(int64_t n)const{
		return pop(n*log2_N_CHAR);
	}
	static inline Word from_chr(chr_t x){
		return Word((uint64_t)x,0,log2_N_CHAR);
	}
};
#endif

inline Word rev(Word w){
	assert(w.len%log2_N_CHAR==0);
	if(w.len<=log2_N_CHAR)return w;
	unsigned __int128 src=w.to_u128(),dst=0;
	unsigned __int128 mask=((unsigned __int128)1<<log2_N_CHAR)-1;
	for(int64_t rem=w.len;rem>0;rem-=log2_N_CHAR){
		dst=(dst<<log2_N_CHAR)|(src&mask);
		src>>=log2_N_CHAR;
	}
	return Word(dst,w.len);
}
Word all0(int64_t len){
	return Word(0,len*log2_N_CHAR);
}

thread_local int64_t block_maxT;

struct WordUpdateLemma{
	Word w0,w1;
	int64_t s0,s1,sgn,n_step;
	bool is_back;
	// s0 w0 => w1 s1: is_back=0
	// s0 w0 => s1 w1: is_back=1
	static std::optional<WordUpdateLemma> from_v2(const TM &tm,const Word w,const int64_t s,const int64_t sgn,const int64_t maxT=320*10){
		auto opt=from(tm,w,s,sgn,maxT);
		if(opt&&!opt->is_back)opt->w1=rev(opt->w1);
		return opt;
	}
	static std::optional<WordUpdateLemma> from(const TM &tm,const Word w,const int64_t s,const int64_t sgn,const int64_t maxT=320*10){
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
	std::optional<std::tuple<int64_t,int64_t,int64_t>> check_halt_cont(const TM &tm,int64_t maxT){
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

namespace ChrTransform{
thread_local int64_t len_h,len_h_no_lru,lru_n;
thread_local bool add_state_to_h;
void init(){
	len_h=0;
	len_h_no_lru=0;
	lru_n=1;
	add_state_to_h=1;
}
inline Word upd(Word w1,Word w0,int64_t s,int64_t sgn){
	if(!len_h)return w1;
	if(add_state_to_h){
		assert(0<=s&&s<N_STATE);
		assert(w0.len%log2_N_CHAR==0);
		if(w0.len==log2_N_CHAR){
			w0=Word((uint64_t)s|((uint64_t)w0.lo<<log2_N_STATE),0,log2_N_STATE+log2_N_CHAR);
		}else{
			w0=Word(s,log2_N_STATE)+(Word(sgn==-1,1)+w0);
		}
	}
	int64_t len=w0.len;
	int64_t new_len=len+w1.len;
	if(new_len>MAX_WORD_LENGTH)throw WordLenError();
	unsigned __int128 d=w0.to_u128()|(w1.to_u128()<<len);
	if(new_len>len_h_no_lru*len){
		unsigned __int128 mask=((unsigned __int128)1<<len)-1;
		int64_t shift0=len_h_no_lru*len;
		unsigned __int128 target=(shift0>=MAX_WORD_LENGTH)?0:((d>>shift0)&mask);
		for(int64_t i=len_h_no_lru+1,j=1,sh=(len_h_no_lru+1)*len;sh<=new_len;++i,sh+=len){
			unsigned __int128 chunk=(sh>=MAX_WORD_LENGTH)?0:((d>>sh)&mask);
			if(chunk==target){
				if(j<lru_n){
					++j;
					continue;
				}
				unsigned __int128 lo_mask=(sh==0)?0:(((unsigned __int128)1<<sh)-1);
				unsigned __int128 high=(sh+len>=MAX_WORD_LENGTH)?0:((d>>(sh+len))<<sh);
				d=(d&lo_mask)|high;
				new_len=sh+std::max<int64_t>(0L,new_len-sh-len);
				break;
			}
		}
	}
	int64_t max_len=len_h*len;
	if(new_len>max_len){
		unsigned __int128 mask=(max_len==0)?0:(((unsigned __int128)1<<max_len)-1);
		d&=mask;
		new_len=max_len;
	}
	return Word(d,new_len);
}

}
using ChrTransform::upd;

namespace CTL_Fixedlen_verify{
struct DFA{
	std::map<std::pair<Word,size_t>,size_t> push_;
	std::unordered_map<size_t,std::set<std::pair<Word,size_t>>> pop_;
	size_t push(Word w,size_t ls){
		return push_[{w,ls}];
	}
	size_t push_all(std::vector<chr_t> w,size_t ls,size_t len){
		std::reverse(w.begin(),w.end());
		while(w.size()%len)w.emplace_back(0);
		std::reverse(w.begin(),w.end());
		size_t sz=w.size();
		for(size_t i=0;i<sz;i+=len){
			Word w0=all0(len);
			for(size_t j=0;j<len;++j){
				w0=w0.set_chr(j,w[i+len-1-j]);
			}
			ls=push(w0,ls);
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
	bool verify(const TM &tm,size_t len,const DH_Tape &t0){
		block_maxT=0;
		bool flag=0;
		if(!(lset.push(all0(len),l0)==l0))return 0;
		if(!(rset.push(all0(len),r0)==r0))return 0;
		auto l1=lset.push_all((t0.sgn==1?t0.l:t0.r),l0,len);
		auto r1=rset.push_all((t0.sgn==1?t0.r:t0.l),r0,len);
		if(t0.sgn==-1)std::swap(l1,r1);
		if(!q.count(Tape{l1,r1,t0.s,t0.sgn}))return 0;
		for(auto [l,r,s,sgn]:q){
			if(!l)return 0;
			if(!r)return 0;
			if(!(0<=s&&s<N_STATE))return 0;
			if(!(sgn==1||sgn==-1))return 0;
			for(auto [w,r1]:(sgn==1?rset:lset).pop_[r]){
				auto [w0,w1]=w.pop_chrs(len);
				auto res=WordUpdateLemma::from(tm,w0,s,sgn);
				if(!res)return 0;
				if(res->s1==-1)return 0;
				w1=upd(w1,w0,s,sgn);
				if(res->is_back){
					Tape t{(sgn==1?rset:lset).push(res->w1+w1,r1),l,res->s1,-sgn};
					if(!q.count(t))return 0;
				}else{
					Tape t{(sgn==1?lset:rset).push(rev(res->w1)+w1,l),r1,res->s1,sgn};
					if(!q.count(t))return 0;
				}
			}
		}
		if(0)std::cerr<<"verified:"
		<<" lset:"<<lset.pop_.size()<<" lset_tr:"<<lset.push_.size()
		<<" rset:"<<rset.pop_.size()<<" rset_tr:"<<rset.push_.size()
		<<" accept_set:"<<q.size()
		<<" block_maxT:"<<block_maxT
		<<'\n';
		return 1;
	}
};
}

namespace MitM_CTL{
thread_local bool CTL_skip_verify=0;
inline size_t hash_combine_u64(size_t h,size_t k){
	h^=k+0x9e3779b97f4a7c15ULL+(h<<6)+(h>>2);
	return h;
}
inline size_t hash_word_vec(const std::vector<Word> &v){
	size_t h=v.size()*0x9e3779b97f4a7c15ULL;
	for(const auto &w:v)h=hash_combine_u64(h,w.hash());
	return h;
}
inline size_t hash_word_pair_vec(const std::vector<std::pair<Word,Word>> &v){
	size_t h=v.size()*0x9e3779b97f4a7c15ULL;
	for(const auto &p:v){
		h=hash_combine_u64(h,p.first.hash());
		h=hash_combine_u64(h,p.second.hash());
	}
	return h;
}


struct LRUpair{
	inline static thread_local int64_t len_h,len_h_no_lru,len_h_tail;
	static void init(){
		len_h=8,len_h_no_lru=2,len_h_tail=0;
	}
	std::vector<Word> q;
	std::vector<std::pair<Word,Word>> lru;
	bool operator<(const LRUpair &w)const{
		return q!=w.q?q<w.q:lru<w.lru;
	}
	bool operator==(const LRUpair &w)const{
		return q==w.q&&lru==w.lru;
	}
	size_t hash()const{
		return hash_combine_u64(hash_word_vec(q),hash_word_pair_vec(lru));
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
	bool operator<(const set_pair &w)const{
		return q!=w.q?q<w.q:lru<w.lru;
	}
	bool operator==(const set_pair &w)const{
		return q==w.q&&lru==w.lru;
	}
	size_t hash()const{
		return hash_combine_u64(hash_word_vec(q),hash_word_pair_vec(lru));
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
	bool operator<(const NGset &w)const{
		return q!=w.q?q<w.q:lru<w.lru;
	}
	bool operator==(const NGset &w)const{
		return q==w.q&&lru==w.lru;
	}
	size_t hash()const{
		return hash_combine_u64(q.hash(),hash_word_vec(lru));
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

struct NG{
	inline static thread_local int64_t NG_n,tH,pos_mod;
	static void init(){
		NG_n=3;
		tH=0;
		pos_mod=1;
	}
	static constexpr int64_t INLINE_CAP=6;
	Word q_buf[INLINE_CAP];
	uint8_t q_sz=0;
	bool use_ovf=false;
	int64_t mod=0;
	std::vector<Word> q_ovf,q0;
	NG()=default;
	NG(const NG &o):q_sz(o.q_sz),use_ovf(o.use_ovf),mod(o.mod){
		if(!use_ovf){
			for(uint8_t i=0;i<q_sz;++i)q_buf[i]=o.q_buf[i];
		}else{
			q_ovf=o.q_ovf;
		}
		if(!o.q0.empty())q0=o.q0;
	}
	NG(NG &&o)noexcept:q_sz(o.q_sz),use_ovf(o.use_ovf),mod(o.mod){
		if(!use_ovf){
			for(uint8_t i=0;i<q_sz;++i)q_buf[i]=o.q_buf[i];
		}else{
			q_ovf=std::move(o.q_ovf);
		}
		if(!o.q0.empty())q0=std::move(o.q0);
	}
	NG &operator=(const NG &o){
		if(this==&o)return *this;
		q_sz=o.q_sz;use_ovf=o.use_ovf;mod=o.mod;
		if(!use_ovf){
			for(uint8_t i=0;i<q_sz;++i)q_buf[i]=o.q_buf[i];
			if(!q_ovf.empty())q_ovf.clear();
		}else{
			q_ovf=o.q_ovf;
		}
		if(!o.q0.empty()||!q0.empty())q0=o.q0;
		return *this;
	}
	NG &operator=(NG &&o)noexcept{
		if(this==&o)return *this;
		q_sz=o.q_sz;use_ovf=o.use_ovf;mod=o.mod;
		if(!use_ovf){
			for(uint8_t i=0;i<q_sz;++i)q_buf[i]=o.q_buf[i];
			if(!q_ovf.empty())q_ovf.clear();
		}else{
			q_ovf=std::move(o.q_ovf);
		}
		if(!o.q0.empty()||!q0.empty())q0=std::move(o.q0);
		return *this;
	}
	inline bool operator==(const NG &w)const{
		if(mod!=w.mod||use_ovf!=w.use_ovf)return false;
		if(!use_ovf){
			if(q_sz!=w.q_sz)return false;
			for(uint8_t i=0;i<q_sz;++i)if(q_buf[i]!=w.q_buf[i])return false;
		}else{
			if(q_ovf!=w.q_ovf)return false;
		}
		return q0.empty()&&w.q0.empty()?true:(q0==w.q0);
	}
	inline size_t hash()const{
		size_t h;
		if(!use_ovf){
			h=(size_t)q_sz*0x9e3779b97f4a7c15ULL;
			for(uint8_t i=0;i<q_sz;++i)h=hash_combine_u64(h,q_buf[i].hash());
		}else{
			h=hash_word_vec(q_ovf);
		}
		if(!q0.empty())h=hash_combine_u64(h,hash_word_vec(q0));
		if(mod)h=hash_combine_u64(h,(size_t)mod);
		return h;
	}
	inline void push(const Word &w){
		if(!use_ovf){
			if(q_sz==0&&!w)return;
			if(NG_n&&q_sz==(uint8_t)NG_n){
				for(uint8_t i=1;i<q_sz;++i)q_buf[i-1]=q_buf[i];
				q_buf[q_sz-1]=w;
			}else if(q_sz<INLINE_CAP){
				q_buf[q_sz++]=w;
			}else{
				use_ovf=true;
				q_ovf.assign(q_buf,q_buf+q_sz);
				q_ovf.emplace_back(w);
			}
		}else{
			if(q_ovf.empty()&&!w)return;
			if((int64_t)q_ovf.size()==NG_n&&NG_n)q_ovf.erase(q_ovf.begin());
			q_ovf.emplace_back(w);
		}
		if(tH&&(int64_t)q0.size()<tH)q0.emplace_back(w);
		if(pos_mod>1)mod=(mod+1)%pos_mod;
		else mod=0;
	}
	friend std::ostream &operator<<(std::ostream &os,const NG &x){
		if(!x.use_ovf){
			std::vector<Word> tmp(x.q_buf,x.q_buf+x.q_sz);
			return os<<tmp<<x.q0<<'_'<<x.mod;
		}
		return os<<x.q_ovf<<x.q0<<'_'<<x.mod;
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
	bool operator<(const RWL_mod &w)const{
		return q<w.q;
	}
	bool operator==(const RWL_mod &w)const{
		return q==w.q;
	}
	size_t hash()const{
		size_t h=q.size()*0x9e3779b97f4a7c15ULL;
		for(const auto &[w0,n,m]:q){
			h=hash_combine_u64(h,w0.hash());
			h=hash_combine_u64(h,((size_t)(uint32_t)n<<32)|(uint32_t)m);
		}
		return h;
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




struct ExRWL{
	inline static thread_local int64_t len_h,len_h_tail,block_size;
	static void init(){
		len_h=8,len_h_tail=0,block_size=1;
	}
	std::vector<Word> q;
	bool operator<(const ExRWL &w)const{
		return q<w.q;
	}
	bool operator==(const ExRWL &w)const{
		return q==w.q;
	}
	size_t hash()const{
		return hash_word_vec(q);
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
	bool operator<(const CPS_LRU &w)const{
		return ls_<w.ls_;
	}
	bool operator==(const CPS_LRU &w)const{
		return ls_==w.ls_;
	}
	size_t hash()const{
		return hash_word_vec(ls_);
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




template<class DFA_state_t>
struct Decider{
	typedef std::tuple<size_t,size_t,int64_t,int64_t> Tape;
	struct QItem{
		uint32_t l,r;
		int8_t s,sgn;
	};
	struct PushEntry{
		uint64_t lo,hi;
		int32_t wlen;
		uint32_t ls;
		uint32_t val;
		uint32_t gen;
	};
	struct IdSlot{
		uint32_t gen;
		uint32_t idx;
	};
	struct DFA{
		std::vector<DFA_state_t> states;
		std::vector<IdSlot> id_buckets;
		size_t id_mask=0;

		std::vector<std::vector<std::pair<Word,size_t>>> pop_;
		size_t pop_used=0;

		std::vector<PushEntry> push_tab;
		size_t push_mask=0;
		size_t push_cnt=0;
		uint32_t cur_gen=0;

		size_t n_upd=0;
		bool upd_flag=0;
		std::vector<std::pair<size_t,std::pair<Word,size_t>>> upds;

		static inline size_t hash_push(const Word &w,size_t ls){
			size_t h=w.hash()^(ls*0x9e3779b97f4a7c15ULL);
			h^=h>>30;
			h*=0xbf58476d1ce4e5b9ULL;
			h^=h>>27;
			return h;
		}
		void rehash_id(size_t new_cap){
			id_buckets.assign(new_cap,IdSlot{0,0});
			id_mask=new_cap-1;
			uint32_t g=cur_gen;
			for(size_t i=1;i<states.size();++i){
				size_t p=states[i].hash()&id_mask;
				while(id_buckets[p].gen==g)p=(p+1)&id_mask;
				id_buckets[p]=IdSlot{g,(uint32_t)i};
			}
		}
		void rehash_push(size_t new_cap){
			std::vector<PushEntry> nt(new_cap,PushEntry{0,0,0,0,0,0});
			size_t nmask=new_cap-1;
			uint32_t g=cur_gen;
			for(const auto &e:push_tab){
				if(e.gen!=g)continue;
				Word w(e.lo,e.hi,e.wlen);
				size_t p=hash_push(w,e.ls)&nmask;
				while(nt[p].gen==g)p=(p+1)&nmask;
				nt[p]=e;
			}
			push_tab.swap(nt);
			push_mask=nmask;
		}
		inline void reset(){
			states.clear();
			++cur_gen;
			if(cur_gen==0){
				cur_gen=1;
				id_buckets.assign(128,IdSlot{0,0});
				push_tab.assign(256,PushEntry{0,0,0,0,0,0});
			}else{
				if(id_buckets.empty()||id_buckets.size()>4096)id_buckets.assign(128,IdSlot{0,0});
				if(push_tab.empty()||push_tab.size()>8192)push_tab.assign(256,PushEntry{0,0,0,0,0,0});
			}
			id_mask=id_buckets.size()-1;
			push_mask=push_tab.size()-1;

			for(size_t i=0;i<pop_used;++i)pop_[i].clear();
			pop_used=0;

			push_cnt=0;
			n_upd=0;
			upd_flag=0;
			upds.clear();
		}
		inline size_t push(const Word &w,size_t ls){
			if((push_cnt+1)*2>=push_tab.size())rehash_push(push_tab.size()*2);
			uint32_t g=cur_gen;
			size_t p=hash_push(w,ls)&push_mask;
			for(;;){
				auto &e=push_tab[p];
				if(e.gen!=g){
					DFA_state_t ls_=states[ls];
					ls_.push(w);
					size_t ls0=get_id(std::move(ls_));
					e.lo=w.lo;
					e.hi=w.hi;
					e.wlen=(int32_t)w.len;
					e.ls=(uint32_t)ls;
					e.val=(uint32_t)ls0;
					e.gen=g;
					++push_cnt;
					if(ls0>=pop_.size())pop_.resize(ls0+1);
					if(ls0+1>pop_used)pop_used=ls0+1;
					pop_[ls0].emplace_back(w,ls);
					upd_flag=1;
					++n_upd;
					upds.emplace_back(ls0,std::make_pair(w,ls));
					return ls0;
				}
				if(e.ls==(uint32_t)ls&&e.lo==w.lo&&e.hi==w.hi&&e.wlen==(int32_t)w.len)return e.val;
				p=(p+1)&push_mask;
			}
		}
		size_t push_all(const std::vector<chr_t> &w_in,size_t ls,size_t len){
			if(w_in.empty())return ls;
			std::vector<chr_t> w=w_in;
			std::reverse(w.begin(),w.end());
			while(w.size()%len)w.emplace_back(0);
			std::reverse(w.begin(),w.end());
			size_t sz=w.size();
			for(size_t i=0;i<sz;i+=len){
				Word w0=all0(len);
				for(size_t j=0;j<len;++j){
					w0=w0.set_chr(j,w[i+len-1-j]);
				}
				ls=push(w0,ls);
				if(!ls)return 0;
			}
			return ls;
		}
		inline size_t get_id(DFA_state_t &&ls){
			if(states.size()*2>=id_buckets.size())rehash_id(id_buckets.size()*2);
			uint32_t g=cur_gen;
			size_t h=ls.hash();
			size_t p=h&id_mask;
			for(;;){
				IdSlot sl=id_buckets[p];
				if(sl.gen!=g){
					size_t nid=states.size();
					states.emplace_back(std::move(ls));
					id_buckets[p]=IdSlot{g,(uint32_t)nid};
					return nid;
				}
				if(states[sl.idx]==ls)return sl.idx;
				p=(p+1)&id_mask;
			}
		}
		inline void init(size_t len){
			reset();
			states.emplace_back();
			auto id0=get_id(DFA_state_t{}),id1=id0;
			do id1=push(all0(len),id1); while(id1!=id0);
		}
		std::map<std::pair<Word,size_t>,size_t> to_push_map()const{
			std::map<std::pair<Word,size_t>,size_t> mp;
			uint32_t g=cur_gen;
			for(const auto &e:push_tab){
				if(e.gen==g)mp[{Word(e.lo,e.hi,e.wlen),e.ls}]=e.val;
			}
			return mp;
		}
		friend std::ostream &operator<<(std::ostream &os,const DFA &x){
			for(size_t i=1;i<x.states.size();++i){
				merr<<x.states.at(i)<<":\n";
				if(i<x.pop_used)
				for(auto [w,r1]:x.pop_[i]){
					merr<<w<<" => "<<x.states.at(r1)<<'\n';
				}
			}
			return os;
		}
	}lset,rset;
	std::vector<QItem> q;
	int64_t used_T=0;
	
	size_t len=1;
	
	std::vector<std::vector<uint64_t>> inqs[2];
	size_t inqs_used[2]={0,0};
	std::vector<uint64_t> tmp_ts;

	inline void reset_decider(){
		q.clear();
		used_T=0;
		for(int side=0;side<2;++side){
			for(size_t i=0;i<inqs_used[side];++i)inqs[side][i].clear();
			inqs_used[side]=0;
		}
	}
	inline void push(QItem t){
		int side=(t.sgn==1?1:0);
		auto &vec=inqs[side];
		if(t.r>=vec.size())vec.resize(t.r+1);
		if((size_t)t.r+1>inqs_used[side])inqs_used[side]=(size_t)t.r+1;
		uint64_t key=((uint64_t)t.l<<8)|(uint64_t)(uint8_t)t.s;
		auto &v=vec[t.r];
		auto it=std::lower_bound(v.begin(),v.end(),key);
		if(it==v.end()||*it!=key){
			v.insert(it,key);
			q.push_back(t);
		}
	}
	
	void print_Tape(QItem t){
		size_t l=t.l,r=t.r;
		int64_t s=t.s,sgn=t.sgn;
		if(sgn==-1)std::swap(l,r);
		merr<<lset.states.at(l);
		merr<<" ";
		if(sgn==-1)merr<<"<";
		merr<<char('A'+s);
		if(sgn==1)merr<<">";
		merr<<" ";
		merr<<rset.states.at(r);
	}
	#define dbg1 dbg
	inline bool step(const TM &tm,QItem t,const std::pair<Word,size_t> &rs,QItem &out_t){
		if(dbg){
			merr<<"step: ";
			print_Tape(t);
			merr<<"\n";
		}
		const auto &[w,r1]=rs;
		int64_t s=t.s,sgn=t.sgn;
		if(len==1){
			chr_t in=w.get_chr(0);
			const Trans &tr=tm.trans[s][in];
			if(tr.nxt==-1){
				if(dbg1)merr<<w<<" may halt\n"<<used_T<<'\n';
				return false;
			}
			Word w1(w.to_u128()>>log2_N_CHAR,std::max<int64_t>(0L,w.len-log2_N_CHAR));
			if(ChrTransform::len_h)w1=upd(w1,Word::from_chr(in),s,sgn);
			Word out_w=Word::from_chr(tr.out)+w1;
			if(tr.dir*sgn<0){
				out_t=QItem{(uint32_t)(sgn==1?rset:lset).push(out_w,r1),t.l,(int8_t)tr.nxt,(int8_t)(-sgn)};
			}else{
				out_t=QItem{(uint32_t)(sgn==1?lset:rset).push(out_w,t.l),(uint32_t)r1,(int8_t)tr.nxt,(int8_t)sgn};
			}
			return true;
		}
		auto [w0,w1]=w.pop_chrs(len);
		auto res=WordUpdateLemma::from(tm,w0,s,sgn);
		if(!res){
			if(dbg1)merr<<w0<<char('A'+s)<<sgn<<"block timeout\n"<<used_T<<'\n';
			return false;
		}
		if(res->s1==-1){
			if(dbg1)merr<<*res<<"may halt\n"<<used_T<<'\n';
			return false;
		}
		w1=upd(w1,w0,s,sgn);
		if(res->is_back){
			out_t=QItem{(uint32_t)(sgn==1?rset:lset).push(res->w1+w1,r1),t.l,(int8_t)res->s1,(int8_t)(-sgn)};
		}else{
			out_t=QItem{(uint32_t)(sgn==1?lset:rset).push(rev(res->w1)+w1,t.l),(uint32_t)r1,(int8_t)res->s1,(int8_t)sgn};
		}
		return true;
	}
	int64_t decide(const TM &tm,int64_t maxT,const DH_Tape &tape0){
		try{
			return decide_(tm,maxT,tape0);
		}catch(WordLenError e){
			return 0;
		}
	}
	int64_t decide_(const TM &tm,int64_t maxT,const DH_Tape &tape0){
		reset_decider();
		lset.init(len);
		rset.init(len);
		auto l1=lset.push_all((tape0.sgn==1?tape0.l:tape0.r),1,len);
		auto r1=rset.push_all((tape0.sgn==1?tape0.r:tape0.l),1,len);
		if(tape0.sgn==-1)std::swap(l1,r1);
		push(QItem{(uint32_t)l1,(uint32_t)r1,(int8_t)tape0.s,(int8_t)tape0.sgn});
		QItem t0;
		for(;;){
			if(!q.empty()){
				QItem t=q.back();
				q.pop_back();
				auto &d=(t.sgn==1?rset:lset);
				if(t.r<d.pop_used){
					size_t sz=d.pop_[t.r].size();
					for(size_t i=0;i<sz;++i){
						auto rs0=d.pop_[t.r][i];
						if(!step(tm,t,rs0,t0))return 0;
						push(t0);
					}
				}
			}else if(!lset.upds.empty()){
				auto [id0,rs0]=lset.upds.back();
				lset.upds.pop_back();
				if(id0<inqs_used[0]&&!inqs[0][id0].empty()){
					tmp_ts.assign(inqs[0][id0].begin(),inqs[0][id0].end());
					for(uint64_t key:tmp_ts){
						QItem t{(uint32_t)(key>>8),(uint32_t)id0,(int8_t)(key&0xFF),-1};
						if(!step(tm,t,rs0,t0))return 0;
						push(t0);
					}
				}
			}else if(!rset.upds.empty()){
				auto [id0,rs0]=rset.upds.back();
				rset.upds.pop_back();
				if(id0<inqs_used[1]&&!inqs[1][id0].empty()){
					tmp_ts.assign(inqs[1][id0].begin(),inqs[1][id0].end());
					for(uint64_t key:tmp_ts){
						QItem t{(uint32_t)(key>>8),(uint32_t)id0,(int8_t)(key&0xFF),1};
						if(!step(tm,t,rs0,t0))return 0;
						push(t0);
					}
				}
			}else{
				if(!CTL_skip_verify){
					std::set<Tape> inq;
					for(int side=0;side<2;++side){
						int64_t sg=(side==1?1:-1);
						for(size_t r=0;r<inqs_used[side];++r){
							for(uint64_t key:inqs[side][r]){
								inq.insert(Tape{(size_t)(key>>8),r,(int64_t)(key&0xFF),sg});
							}
						}
					}
					assert(CTL_Fixedlen_verify::Verifier(lset.to_push_map(),rset.to_push_map(),inq,1,1).verify(tm,len,tape0));
				}
				if(dbg1)std::cerr<<'\n'<<tm<<" N"<<" T:"<<used_T<<'\n';
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

namespace Basic0{
struct Tape{
	std::vector<chr_t> l,r;
	chr_t in=0;
	int64_t s=0;
	bool step(const TM &tm){
		if(s==-1)return 1;
		Trans tr=tm.trans[s][in];
		s=tr.nxt;
		if(tr.dir==1){
			l.push_back(tr.out);
			in=pops1(r);
		}else{
			r.push_back(tr.out);
			in=pops1(l);
		}
		return 0;
	}
	void simpl(){
		if(l.size()==1&&!l.back())l.clear();
		if(r.size()==1&&!r.back())r.clear();
	}
	int64_t check_halt(const TM &tm,int64_t maxT){
		for(int64_t i=1;i<=maxT;++i){
			if(step(tm))return i;
		}
		return -1;
	}
	std::optional<std::pair<int64_t,int64_t>> check_translated_cycler_v2(const TM &tm,int64_t maxT,int64_t maxS){
		typedef std::pair<uint64_t,int64_t> H;
		std::vector<H> hl,hr;
		H h0{0,0};
		H hl0=h0,hr0=h0;
		auto upd=[](H &h,uint64_t x){
			h.first=h.first*12347+x;
			++h.second;
		};
		auto chk=[](const std::vector<H> &h)->std::optional<std::pair<int64_t,int64_t>>{
			if(h.empty())return std::nullopt;
			int64_t T=h.size();
			for(int64_t d=1;T-d*2>=0;++d){
				bool flag=1;
				for(int64_t i=0;i<d;++i){
					if(h[T-d*2+i]!=h[T-d+i]){flag=0;break;}
				}
				if(!flag)continue;
				int64_t S=0,P=0;
				while(T-d*2>0&&h[T-d*2-1]==h[T-d-1])--T;
				for(int64_t i=0;i<T-d*2;++i)S+=h[i].second;
				for(int64_t i=0;i<d;++i)P+=h[T-d*2+i].second;
				return std::make_pair(S,P);
			}
			return std::nullopt;
		};
		for(int64_t i=1;i<=maxT;++i){
			if(s==-1)return std::nullopt;
			upd(hl0,s*N_CHAR+in);
			upd(hr0,s*N_CHAR+in);
			Trans tr=tm.trans[s][in];
			s=tr.nxt;
			if(tr.dir==1){
				l.push_back(tr.out);
				if(!r.empty()){
					in=r.back();
					r.pop_back();
				}else{
					in=false;
					hr.emplace_back(hr0);
					if(dbg){
						constexpr int64_t C=200;
						if(l.size()>=C&&hr0.second>0){
							bool flag=0;
							for(int64_t i=0;i<C;++i){
								auto v=int(l[l.size()-C+i]);
								if(!v)flag=1;
								if(flag)merr<<v;
							}
							merr<<" "<<char('A'+s)<<" "<<hr0.second<<" "<<i<<'\n';
						}
					}
					hr0=h0;
					if(hl.size()+hr.size()>=maxS){
						if(dbg)std::cerr<<"T="<<i<<" left tape overflow\n";
						break;
					}
				}
			}else{
				r.push_back(tr.out);
				if(!l.empty()){
					in=l.back();
					l.pop_back();
				}else{
					in=false;
					hl.emplace_back(hl0);
					hl0=h0;
					if(hl.size()+hr.size()>=maxS){
						if(dbg)std::cerr<<"T="<<i<<" right tape overflow\n";
						break;
					}
				}
			}
		}
		if(auto res=chk(hl))return res;
		if(auto res=chk(hr))return res;
		return std::nullopt;
	}
	std::optional<std::pair<int64_t,int64_t>> check_translated_cycler(const TM &tm,int64_t maxT,int64_t maxT0){
		std::vector<uint8_t> h;
		h.reserve(maxT0);
		for(int64_t i=1;i<=maxT;++i){
			if(s==-1)return std::nullopt;
			h.emplace_back(((s*N_CHAR+in)*2+(l.empty()))*2+(r.empty()));
			if(h.size()>=maxT0){
				if(l.size()+r.size()>=maxT0*8)return std::nullopt;
				int64_t T=h.size()-1;
				for(int64_t d=1;T-d*2>=0;++d){
					if(!eq3(h[T],h[T-d],h[T-d*2]))continue;
					bool flag=1;
					if(memcmp(h.data()+T-d,h.data()+T-d*2,d))flag=0;
					/*
					for(int64_t j=0;j<=d;++j){
						if(h[T-d+j]!=h[T-d*2+j]){flag=0;break;}
					}*/
					if(!flag)continue;
					int64_t p=T-d*2;
					for(;;){
						if(h[p-1+d]&3){
							return std::make_pair(i,d);
						}
						if(!(p>0&&h[p-1]==h[p-1+d]))goto o1;
						--p;
					}
					o1:;
				}
				h.clear();
			}
			step(tm);
		}
		return std::nullopt;
	}
};
}


namespace Basic{
bool pops1(std::vector<bool> &xs){
	bool c=0;
	if(!xs.empty()){
		c=xs.back();
		xs.pop_back();
	}
	return c;
}
struct Tape{
	std::vector<bool> l,r;
	bool in=0;
	int64_t s=0,pos=0;
	Tape(){
		assert(N_CHAR==2);
	}
	bool step(const TM &tm){
		if(s==-1)return 1;
		Trans tr=tm.trans[s][in];
		s=tr.nxt;
		if(tr.dir==1){
			l.push_back(tr.out);
			in=pops1(r);
		}else{
			r.push_back(tr.out);
			in=pops1(l);
		}
		pos+=tr.dir;
		return 0;
	}
	int64_t check_halt(const TM &tm,int64_t maxT){
		for(int64_t i=1;i<=maxT;++i){
			if(step(tm))return i;
		}
		return -1;
	}
	bool check_bouncer(const TM &tm,int64_t maxT,int64_t maxS){
		std::vector<int64_t> hl,hr;
		int64_t hl0=0,hr0=0;
		auto chk=[](const std::vector<int64_t> &h)->bool{
			if(h.empty())return 0;
			int64_t T=h.size();
			for(int64_t d=1;T-d*5>=10;++d){
				bool flag=1;
				for(int64_t i=0;i<d;++i){
					int64_t v0=h[T-d*5+i];
					int64_t v1=h[T-d*4+i];
					int64_t v2=h[T-d*3+i];
					int64_t v3=h[T-d*2+i];
					int64_t v4=h[T-d*1+i];
					if(!eq4(v1-v0,v2-v1,v3-v2,v4-v3)){flag=0;break;}
				}
				if(!flag)continue;
				return 1;
			}
			return 0;
		};
		for(int64_t i=1;i<=maxT;++i){
			if(s==-1)return 0;
			Trans tr=tm.trans[s][in];
			s=tr.nxt;
			if(tr.dir==1){
				l.push_back(tr.out);
				if(!r.empty()){
					in=r.back();
					r.pop_back();
				}else{
					in=false;
					hr.emplace_back(i-hr0);
					hr0=i;
					if(hl.size()+hr.size()>=maxS){
						if(dbg)std::cerr<<"T="<<i<<" left tape overflow\n";
						break;
					}
				}
			}else{
				r.push_back(tr.out);
				if(!l.empty()){
					in=l.back();
					l.pop_back();
				}else{
					in=false;
					hl.emplace_back(i-hl0);
					hl0=i;
					if(hl.size()+hr.size()>=maxS){
						if(dbg)std::cerr<<"T="<<i<<" right tape overflow\n";
						break;
					}
				}
			}
		}
		if(hl0>maxT/4)if(auto res=chk(hl))return res;
		if(hr0>maxT/4)if(auto res=chk(hr))return res;
		return 0;
	}
	std::optional<std::pair<int64_t,int64_t>> check_translated_cycler_v2(const TM &tm,int64_t maxT,int64_t maxS){
		typedef std::pair<uint64_t,int64_t> H;
		std::vector<H> hl,hr;
		H h0{0,0};
		H hl0=h0,hr0=h0;
		auto upd=[](H &h,uint64_t x){
			h.first=h.first*12347+x;
			++h.second;
		};
		auto chk=[](const std::vector<H> &h)->std::optional<std::pair<int64_t,int64_t>>{
			if(h.empty())return std::nullopt;
			int64_t T=h.size();
			for(int64_t d=1;T-d*2>=0;++d){
				bool flag=1;
				for(int64_t i=0;i<d;++i){
					if(h[T-d*2+i]!=h[T-d+i]){flag=0;break;}
				}
				if(!flag)continue;
				int64_t S=0,P=0;
				while(T-d*2>0&&h[T-d*2-1]==h[T-d-1])--T;
				for(int64_t i=0;i<T-d*2;++i)S+=h[i].second;
				for(int64_t i=0;i<d;++i)P+=h[T-d*2+i].second;
				return std::make_pair(S,P);
			}
			return std::nullopt;
		};
		for(int64_t i=1;i<=maxT;++i){
			if(s==-1)return std::make_pair(i,0);
			if(dbg&&i%100000000==0)std::cerr<<i/100000000<<"\t"<<(l.size()+r.size()+1)<<'\n';
			upd(hl0,s*N_CHAR+in);
			upd(hr0,s*N_CHAR+in);
			Trans tr=tm.trans[s][in];
			s=tr.nxt;
			if(tr.dir==1){
				l.push_back(tr.out);
				if(!r.empty()){
					in=r.back();
					r.pop_back();
				}else{
					in=false;
					hr.emplace_back(hr0);
					/*if(dbg){
						constexpr int64_t C=200;
						if(l.size()>=C&&hr0.second>0){
							bool flag=0;
							for(int64_t i=0;i<C;++i){
								auto v=int(l[l.size()-C+i]);
								if(!v)flag=1;
								if(flag)merr<<v;
							}
							merr<<" "<<char('A'+s)<<" "<<hr0.second<<" "<<i<<'\n';
						}
					}*/
					hr0=h0;
					if(hl.size()+hr.size()>=maxS){
						if(dbg)std::cerr<<"T="<<i<<" left tape overflow\n";
						break;
					}
				}
			}else{
				r.push_back(tr.out);
				if(!l.empty()){
					in=l.back();
					l.pop_back();
				}else{
					in=false;
					hl.emplace_back(hl0);
					hl0=h0;
					if(hl.size()+hr.size()>=maxS){
						if(dbg)std::cerr<<"T="<<i<<" right tape overflow\n";
						break;
					}
				}
			}
		}
		if(auto res=chk(hl))return res;
		if(auto res=chk(hr))return res;
		if(hl.size()+hr.size()>=maxS)std::cerr<<'\n'<<tm<<" OOM        \n";
		return std::nullopt;
	}
	std::optional<std::pair<int64_t,int64_t>> check_translated_cycler(const TM &tm,int64_t maxT,int64_t maxT0){
		std::vector<uint8_t> h;
		h.reserve(maxT0);
		for(int64_t i=1;i<=maxT;++i){
			if(s==-1)return std::nullopt;
			h.emplace_back(((s*N_CHAR+in)*2+(l.empty()))*2+(r.empty()));
			if(h.size()>=maxT0){
				if(l.size()+r.size()>=maxT0*8)return std::nullopt;
				int64_t T=h.size()-1;
				for(int64_t d=1;T-d*2>=0;++d){
					if(!eq3(h[T],h[T-d],h[T-d*2]))continue;
					bool flag=1;
					if(memcmp(h.data()+T-d,h.data()+T-d*2,d))flag=0;
					/*
					for(int64_t j=0;j<=d;++j){
						if(h[T-d+j]!=h[T-d*2+j]){flag=0;break;}
					}*/
					if(!flag)continue;
					int64_t p=T-d*2;
					for(;;){
						if(h[p-1+d]&3){
							return std::make_pair(i,d);
						}
						if(!(p>0&&h[p-1]==h[p-1+d]))goto o1;
						--p;
					}
					o1:;
				}
				h.clear();
			}
			step(tm);
		}
		return std::nullopt;
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

struct Word_hash{
	size_t operator()(const Word &x)const{
		return x.hash();
	}
};
#define NEQ(x,y) y!=x.y||
#define NEQ0(x,y) y!=x.y;
#define CMP(x,y) y!=x.y?y<x.y:
#define CMP0(x,y) y<x.y;

void execw2(const TM &tm,Word w[2],int64_t &s,int64_t &pos){
	while(pos==0||pos==1){
		int64_t dir=pos*2-1;
		auto res=WordUpdateLemma::from_v2(tm,w[pos],s,pos*2-1);
		if(!res)throw int64_t(-6);
		s=res->s1;
		w[pos]=res->w1;
		pos+=(res->is_back?-dir:dir);
		if(s==-1)throw int64_t(1);
	}
}

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
	size_t n=1,maxT=1,maxS=1e8;
	int64_t result=0;
	std::map<std::string,std::function<void()>> cmd_mp;
	void add_cmd(std::string cmd,std::function<void()> f){
		cmd_mp[cmd]=f;
	}
	LineExec(const TM &tm,std::string ln_):ln(ln_),ss(ln),tm(tm){
		init();
	}
	LineExec(std::string ln_):ln(ln_),ss(ln){
		if(!(ss>>tm))assert(0);
		init();
	}
	void init(){
		//std::cerr<<'\n'<<ln<<'\n';
		
		ChrTransform::init();
		def_var(chr_LRUH,ChrTransform::len_h);
		def_var(chr_H,ChrTransform::len_h_no_lru);
		def_var(chr_LRUn,ChrTransform::lru_n);
		def_var(chr_asth,ChrTransform::add_state_to_h);
		
		add_cmd("TC",[this]{
			def_var(maxT,maxT);
			def_var(maxS,maxS);
			add_cmd("run",[this]{
				Basic::Tape t;
				auto o=t.check_translated_cycler_v2(tm,maxT,maxS);
				if(o){
					std::cerr<<'\n'<<tm<<' '<<o->first<<' '<<o->second<<"            "<<'\n';
					throw int64_t(o->second?2:1);
				}
				throw (int64_t)0;
			});
		});
		
		add_cmd("MitM_CTL",[this]{
			def_var(n,n);
			def_var(maxT,maxT);
			add_cmd("sim",[this]{
				size_t sim_maxT=0;
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
					f.len=n;
					throw f.decide(tm,maxT,tape);
				});
			});
			
			add_cmd("set_pair",[this]{
				set_pair::init();
				def_var(setH,set_pair::len_h);
				def_var(H,set_pair::len_h_no_lru);
				def_var(tH,set_pair::len_h_tail);
				add_cmd("run",[this]{
					MitM_CTL::Decider<set_pair> f;
					f.len=n;
					throw f.decide(tm,maxT,tape);
				});
			});

			add_cmd("NGset",[this]{
				NGset::init();
				def_var(NG_n,NGset::NG_n);
				def_var(H,NGset::len_h);
				add_cmd("run",[this]{
					MitM_CTL::Decider<NGset> f;
					f.len=n;
					throw f.decide(tm,maxT,tape);
				});
			});

			add_cmd("NG",[this]{
				NG::init();
				def_var(NG_n,NG::NG_n);
				def_var(tH,NG::tH);
				def_var(pos_mod,NG::pos_mod);
				add_cmd("run",[this]{
					MitM_CTL::Decider<NG> f;
					f.len=n;
					throw f.decide(tm,maxT,tape);
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
					f.len=n;
					throw f.decide(tm,maxT,tape);
				});
			});

			add_cmd("ExRWL",[this]{
				ExRWL::init();
				def_var(H,ExRWL::len_h);
				def_var(tH,ExRWL::len_h_tail);
				def_var(bsz,ExRWL::block_size);
				add_cmd("run",[this]{
					MitM_CTL::Decider<ExRWL> f;
					f.len=n;
					throw f.decide(tm,maxT,tape);
				});
			});

			add_cmd("CPS_LRU",[this]{
				CPS_LRU::init();
				def_var(LRUH,CPS_LRU::len_h);
				def_var(H,CPS_LRU::len_h_no_lru);
				def_var(tH,CPS_LRU::len_h_tail);
				add_cmd("run",[this]{
					MitM_CTL::Decider<CPS_LRU> f;
					f.len=n;
					throw f.decide(tm,maxT,tape);
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

std::mutex mtx;
std::vector<std::string> todo;
int main_verify(int argc,char **argv,bool may_fail){
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
	#pragma omp parallel for schedule(dynamic) num_threads(1)
	for(size_t i=0;i<sz;++i){
		{
			std::lock_guard _(mtx);
			++n_tot;
		}
		if(app_stopped)assert(0);
		LineExec exec(todo[i]);
		if(exec.result==2){
			std::lock_guard _(mtx);
			++n_decided;
			show_status();
			merr<<exec.tm<<" N\n";
		}else if(exec.result==1){
			std::lock_guard _(mtx);
			++n_decided;
			show_status();
			merr<<exec.tm<<" H\n";
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

constexpr int64_t T_ckpt=10000;

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
			if(mt()%3||i<a.n_visited){
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
	std::shuffle(ls.begin(),ls.end(),mt);
	UCB ucb;
	ucb.init(ls);
	ucb.load();
	//ucb.ignore(read_file("ucb_2x5_log.txt"));
	ucb.ignore(read_file("ucb_log.txt"));
	for(;;){
		#pragma omp parallel for schedule(dynamic) num_threads(3)
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


struct Decider64{
	static constexpr uint64_t WORD_MASK=(1ULL<<57)-1;
	struct NG64{
		uint64_t a,b,c;
		inline bool operator==(const NG64 &o)const{return a==o.a&&b==o.b&&c==o.c;}
		inline size_t hash()const{
			uint64_t h=a^(b*0x9e3779b97f4a7c15ULL)^(c*0xbf58476d1ce4e5b9ULL);
			h^=h>>30;h*=0xbf58476d1ce4e5b9ULL;h^=h>>27;
			return (size_t)h;
		}
		inline void push(uint64_t w,size_t ng_n){
			if(a==0&&((w&WORD_MASK)==0))return;
			if(ng_n==1){a=w;}
			else if(ng_n==2){if(a==0)a=w;else if(b==0)b=w;else{a=b;b=w;}}
			else{if(a==0)a=w;else if(b==0)b=w;else if(c==0)c=w;else{a=b;b=c;c=w;}}
		}
	};
	struct PushEntry64{
		uint64_t w;
		uint32_t ls_val; // (ls<<16)|val
		uint32_t gen;
	};
	struct IdSlot{
		uint32_t gen;
		uint32_t idx;
	};
	struct PopEdge{
		uint64_t w;
		uint32_t r1;
	};
	struct UpdEdge{
		uint64_t w;
		uint32_t id0;
		uint32_t r1;
	};
	struct QItem{
		uint32_t l,r;
		int8_t s,sgn;
	};
	struct DFA64{
		std::vector<NG64> states;
		std::vector<IdSlot> id_buckets;
		size_t id_mask=0;
		std::vector<std::vector<PopEdge>> pop_;
		size_t pop_used=0;
		std::vector<PushEntry64> push_tab;
		size_t push_mask=0;
		size_t push_cnt=0;
		uint32_t cur_gen=0;
		size_t n_upd=0;
		std::vector<UpdEdge> upds;

		static inline size_t hash_push(uint64_t w,uint32_t ls){
			uint64_t h=w^((uint64_t)ls*0x9e3779b97f4a7c15ULL);
			h^=h>>30;h*=0xbf58476d1ce4e5b9ULL;h^=h>>27;
			return (size_t)h;
		}
		void rehash_id(size_t new_cap){
			id_buckets.assign(new_cap,IdSlot{0,0});
			id_mask=new_cap-1;
			uint32_t g=cur_gen;
			for(size_t i=1;i<states.size();++i){
				size_t p=states[i].hash()&id_mask;
				while(id_buckets[p].gen==g)p=(p+1)&id_mask;
				id_buckets[p]=IdSlot{g,(uint32_t)i};
			}
		}
		void rehash_push(size_t new_cap){
			std::vector<PushEntry64> nt(new_cap,PushEntry64{0,0,0});
			size_t nmask=new_cap-1;
			uint32_t g=cur_gen;
			for(const auto &e:push_tab){
				if(e.gen!=g)continue;
				size_t p=hash_push(e.w,e.ls_val>>16)&nmask;
				while(nt[p].gen==g)p=(p+1)&nmask;
				nt[p]=e;
			}
			push_tab.swap(nt);
			push_mask=nmask;
		}
		inline void init(size_t ng_n){
			states.clear();
			++cur_gen;
			if(cur_gen==0){
				cur_gen=1;
				id_buckets.assign(256,IdSlot{0,0});
				push_tab.assign(512,PushEntry64{0,0,0});
			}else{
				if(id_buckets.empty()||id_buckets.size()>8192)id_buckets.assign(256,IdSlot{0,0});
				if(push_tab.empty()||push_tab.size()>16384)push_tab.assign(512,PushEntry64{0,0,0});
			}
			id_mask=id_buckets.size()-1;
			push_mask=push_tab.size()-1;
			for(size_t i=0;i<pop_used;++i)pop_[i].clear();
			pop_used=0;
			push_cnt=0;
			n_upd=0;
			upds.clear();
			states.push_back(NG64{0,0,0});
			uint32_t id0=get_id(NG64{0,0,0}),id1=id0;
			uint64_t w0=(1ULL<<57);
			do id1=push(w0,id1,ng_n); while(id1!=id0);
		}
		inline uint32_t get_id(const NG64 &ls){
			if(states.size()*2>=id_buckets.size())rehash_id(id_buckets.size()*2);
			uint32_t g=cur_gen;
			size_t p=ls.hash()&id_mask;
			for(;;){
				IdSlot sl=id_buckets[p];
				if(sl.gen!=g){
					uint32_t nid=(uint32_t)states.size();
					states.push_back(ls);
					id_buckets[p]=IdSlot{g,nid};
					return nid;
				}
				if(states[sl.idx]==ls)return sl.idx;
				p=(p+1)&id_mask;
			}
		}
		inline uint32_t push(uint64_t w,uint32_t ls,size_t ng_n){
			if((push_cnt+1)*2>=push_tab.size())rehash_push(push_tab.size()*2);
			uint32_t g=cur_gen;
			size_t p=hash_push(w,ls)&push_mask;
			for(;;){
				auto &e=push_tab[p];
				if(e.gen!=g){
					NG64 ls_=states[ls];
					ls_.push(w,ng_n);
					uint32_t ls0=get_id(ls_);
					e.w=w;
					e.ls_val=(ls<<16)|ls0;
					e.gen=g;
					++push_cnt;
					if(ls0>=pop_.size())pop_.resize(ls0+1);
					if((size_t)ls0+1>pop_used)pop_used=(size_t)ls0+1;
					pop_[ls0].push_back(PopEdge{w,ls});
					++n_upd;
					upds.push_back(UpdEdge{w,ls0,ls});
					return ls0;
				}
				if(e.w==w&&(e.ls_val>>16)==ls)return e.ls_val&0xFFFFu;
				p=(p+1)&push_mask;
			}
		}
	}lset,rset;
	std::vector<QItem> q;
	std::vector<std::vector<uint64_t>> inqs[2];
	size_t inqs_used[2]={0,0};
	std::vector<uint64_t> tmp_ts;

	inline void push_q(QItem t){
		int side=(t.sgn==1?1:0);
		auto &vec=inqs[side];
		if(t.r>=vec.size())vec.resize(t.r+1);
		if((size_t)t.r+1>inqs_used[side])inqs_used[side]=(size_t)t.r+1;
		uint64_t key=((uint64_t)t.l<<8)|(uint64_t)(uint8_t)t.s;
		auto &v=vec[t.r];
		auto it=std::lower_bound(v.begin(),v.end(),key);
		if(it==v.end()||*it!=key){
			v.insert(it,key);
			q.push_back(t);
		}
	}
	static inline uint64_t upd64(uint64_t w,int64_t s,int64_t len_h,int64_t len_h_no_lru,uint8_t out_bit){
		int64_t w_len=(int64_t)(w>>57);
		uint64_t w_bits=w&WORD_MASK;
		uint64_t in=w_bits&1ULL;
		uint64_t w1_bits=w_bits>>1;
		int64_t w1_len=w_len-1;
		uint64_t w0_bits=(uint64_t)s|(in<<3);
		uint64_t d=w0_bits|(w1_bits<<4);
		int64_t new_len=4+w1_len;
		int64_t sh0=len_h_no_lru*4;
		if(new_len>sh0){
			uint64_t target=(d>>sh0)&0xFULL;
			for(int64_t sh=sh0+4;sh<=new_len;sh+=4){
				if(((d>>sh)&0xFULL)==target){
					uint64_t lo_mask=(1ULL<<sh)-1;
					uint64_t high=(sh+4>=64)?0ULL:((d>>(sh+4))<<sh);
					d=(d&lo_mask)|high;
					new_len=sh+(new_len>sh+4?new_len-sh-4:0);
					break;
				}
			}
		}
		int64_t max_len=len_h*4;
		if(new_len>max_len){
			d&=(1ULL<<max_len)-1;
			new_len=max_len;
		}
		return ((uint64_t)(new_len+1)<<57)|(uint64_t)out_bit|(d<<1);
	}
	int64_t decide(const TM &tm,int64_t len_h,int64_t len_h_no_lru,int64_t maxT,size_t ng_n){
		q.clear();
		for(int side=0;side<2;++side){
			for(size_t i=0;i<inqs_used[side];++i)inqs[side][i].clear();
			inqs_used[side]=0;
		}
		lset.init(ng_n);
		rset.init(ng_n);
		push_q(QItem{1,1,0,1});
		int64_t used_T=0;
		for(;;){
			if(!q.empty()){
				QItem t=q.back();
				q.pop_back();
				auto &d=(t.sgn==1?rset:lset);
				auto &o=(t.sgn==1?lset:rset);
				if(t.r<d.pop_used){
					size_t sz=d.pop_[t.r].size();
					for(size_t i=0;i<sz;++i){
						PopEdge pe=d.pop_[t.r][i];
						const Trans &tr=tm.trans[t.s][pe.w&1ULL];
						if(tr.nxt==-1)return 0;
						uint64_t out_w=upd64(pe.w,t.s,len_h,len_h_no_lru,(uint8_t)tr.out);
						if(tr.dir*t.sgn<0){
							uint32_t nl=d.push(out_w,pe.r1,ng_n);
							push_q(QItem{nl,t.l,tr.nxt,(int8_t)(-t.sgn)});
						}else{
							uint32_t nl=o.push(out_w,t.l,ng_n);
							push_q(QItem{nl,pe.r1,tr.nxt,t.sgn});
						}
					}
				}
			}else if(!lset.upds.empty()){
				UpdEdge ue=lset.upds.back();
				lset.upds.pop_back();
				if(ue.id0<inqs_used[0]&&!inqs[0][ue.id0].empty()){
					tmp_ts.assign(inqs[0][ue.id0].begin(),inqs[0][ue.id0].end());
					for(uint64_t key:tmp_ts){
						uint32_t l=(uint32_t)(key>>8);
						int8_t s=(int8_t)(key&0xFF);
						const Trans &tr=tm.trans[s][ue.w&1ULL];
						if(tr.nxt==-1)return 0;
						uint64_t out_w=upd64(ue.w,s,len_h,len_h_no_lru,(uint8_t)tr.out);
						if(tr.dir>0){
							uint32_t nl=lset.push(out_w,ue.r1,ng_n);
							push_q(QItem{nl,l,tr.nxt,1});
						}else{
							uint32_t nl=rset.push(out_w,l,ng_n);
							push_q(QItem{nl,ue.r1,tr.nxt,-1});
						}
					}
				}
			}else if(!rset.upds.empty()){
				UpdEdge ue=rset.upds.back();
				rset.upds.pop_back();
				if(ue.id0<inqs_used[1]&&!inqs[1][ue.id0].empty()){
					tmp_ts.assign(inqs[1][ue.id0].begin(),inqs[1][ue.id0].end());
					for(uint64_t key:tmp_ts){
						uint32_t l=(uint32_t)(key>>8);
						int8_t s=(int8_t)(key&0xFF);
						const Trans &tr=tm.trans[s][ue.w&1ULL];
						if(tr.nxt==-1)return 0;
						uint64_t out_w=upd64(ue.w,s,len_h,len_h_no_lru,(uint8_t)tr.out);
						if(tr.dir<0){
							uint32_t nl=rset.push(out_w,ue.r1,ng_n);
							push_q(QItem{nl,l,tr.nxt,-1});
						}else{
							uint32_t nl=lset.push(out_w,l,ng_n);
							push_q(QItem{nl,ue.r1,tr.nxt,1});
						}
					}
				}
			}else{
				return 2;
			}
			++used_T;
			if(used_T+(int64_t)(q.size()+lset.upds.size()+rset.upds.size())>=maxT)return 0;
			if((int64_t)(lset.n_upd+rset.n_upd)>=maxT)return 0;
		}
	}
};

struct FastTape{
	static constexpr int64_t STACK_RADIUS=304;
	static constexpr int64_t STACK_SIZE=STACK_RADIUS*2+8;
	uint8_t stack_tape[STACK_SIZE];
	uint8_t stack_h[304];
	int64_t head=STACK_RADIUS,min_pos=STACK_RADIUS,max_pos=STACK_RADIUS;
	int64_t s=0,last_s=0,last_in=0,steps=0;
	inline bool run_first_50(const TM &tm){
		memset(stack_tape+STACK_RADIUS-50,0,101);
		uint8_t * __restrict tp=stack_tape;
		uint8_t * __restrict hp=stack_h;
		int64_t hd=STACK_RADIUS,mn=STACK_RADIUS,mx=STACK_RADIUS,st=0,ls=0,li=0;
		for(int64_t i=0;i<50;++i){
			if(st==-1){
				head=hd;min_pos=mn;max_pos=mx;s=st;last_s=ls;last_in=li;steps=i;
				return true;
			}
			uint8_t in=tp[hd];
			hp[i]=(uint8_t)(((st*N_CHAR+in)*2+(hd==mn))*2+(hd==mx));
			const Trans &tr=tm.trans[st][in];
			ls=st;li=in;st=tr.nxt;
			tp[hd]=(uint8_t)tr.out;
			hd+=tr.dir;
			if(hd<mn)mn=hd;
			if(hd>mx)mx=hd;
		}
		head=hd;min_pos=mn;max_pos=mx;s=st;last_s=ls;last_in=li;steps=50;
		return false;
	}
	inline bool run_next_250(const TM &tm){
		memset(stack_tape+STACK_RADIUS-300,0,250);
		memset(stack_tape+STACK_RADIUS+51,0,250);
		uint8_t * __restrict tp=stack_tape;
		uint8_t * __restrict hp=stack_h+50;
		int64_t hd=head,mn=min_pos,mx=max_pos,st=s,ls=last_s,li=last_in;
		for(int64_t i=0;i<250;++i){
			if(st==-1){
				head=hd;min_pos=mn;max_pos=mx;s=st;last_s=ls;last_in=li;steps=50+i;
				return true;
			}
			uint8_t in=tp[hd];
			hp[i]=(uint8_t)(((st*N_CHAR+in)*2+(hd==mn))*2+(hd==mx));
			const Trans &tr=tm.trans[st][in];
			ls=st;li=in;st=tr.nxt;
			tp[hd]=(uint8_t)tr.out;
			hd+=tr.dir;
			if(hd<mn)mn=hd;
			if(hd>mx)mx=hd;
		}
		head=hd;min_pos=mn;max_pos=mx;s=st;last_s=ls;last_in=li;steps=300;
		return false;
	}
	static inline bool check_tc_buf(const uint8_t * __restrict hp,int64_t N,int64_t span){
		if(span>=N*8)return false;
		int64_t T=N-1;
		uint8_t hT=hp[T];
		uint8_t hT1=hp[T-1];
		for(int64_t d=1;T-d*2>=0;++d){
			if(hp[T-d]!=hT||hp[T-d*2]!=hT)continue;
			if(d>1){
				if(hp[T-1-d]!=hT1)continue;
				if(d>2&&memcmp(hp+T-d+1,hp+T-d*2+1,(size_t)(d-2)))continue;
			}
			int64_t p=T-d*2;
			for(;;){
				if(hp[p-1+d]&3)return true;
				if(!(p>0&&hp[p-1]==hp[p-1+d]))break;
				--p;
			}
		}
		return false;
	}
	inline bool check_tc(int64_t N)const{
		return check_tc_buf(stack_h,N,max_pos-min_pos);
	}
};
struct LongTape{
	std::vector<uint8_t> tape;
	std::vector<uint8_t> h;
	int64_t head,min_pos,max_pos,s,last_s,last_in,steps;
	explicit LongTape(const FastTape &ft){
		int64_t init_rad=3200;
		tape.assign(init_rad*2+16,0);
		int64_t shift=init_rad-ft.head;
		memcpy(tape.data()+ft.min_pos+shift,ft.stack_tape+ft.min_pos,(size_t)(ft.max_pos-ft.min_pos+1));
		head=ft.head+shift;min_pos=ft.min_pos+shift;max_pos=ft.max_pos+shift;
		s=ft.s;last_s=ft.last_s;last_in=ft.last_in;steps=ft.steps;
		h.resize(3008);
		memcpy(h.data(),ft.stack_h,(size_t)steps);
	}
	void ensure_capacity(int64_t add_steps,bool record_h){
		int64_t need_steps=steps+add_steps;
		if(head-add_steps-2<0||head+add_steps+2>=(int64_t)tape.size()){
			int64_t new_rad=need_steps+64;
			std::vector<uint8_t> nt(new_rad*2+16,0);
			int64_t shift=new_rad-head;
			memcpy(nt.data()+min_pos+shift,tape.data()+min_pos,(size_t)(max_pos-min_pos+1));
			head+=shift;min_pos+=shift;max_pos+=shift;
			tape.swap(nt);
		}
		if(record_h&&need_steps>(int64_t)h.size()){
			h.resize(need_steps);
		}
	}
	template<bool RecordH>
	bool run_steps(const TM &tm,int64_t n){
		ensure_capacity(n,RecordH);
		uint8_t * __restrict tp=tape.data();
		uint8_t * __restrict hp=h.data();
		int64_t hd=head,mn=min_pos,mx=max_pos,st=s,ls=last_s,li=last_in,sc=steps;
		for(int64_t i=0;i<n;++i){
			if(st==-1){
				head=hd;min_pos=mn;max_pos=mx;s=st;last_s=ls;last_in=li;steps=sc;
				return true;
			}
			uint8_t in=tp[hd];
			if constexpr(RecordH){
				hp[sc]=(uint8_t)(((st*N_CHAR+in)*2+(hd==mn))*2+(hd==mx));
			}
			++sc;
			const Trans &tr=tm.trans[st][in];
			ls=st;li=in;st=tr.nxt;
			tp[hd]=(uint8_t)tr.out;
			hd+=tr.dir;
			if(hd<mn)mn=hd;
			if(hd>mx)mx=hd;
		}
		head=hd;min_pos=mn;max_pos=mx;s=st;last_s=ls;last_in=li;steps=sc;
		return false;
	}
	bool check_tc(int64_t N)const{
		return FastTape::check_tc_buf(h.data(),N,max_pos-min_pos);
	}
};

struct TNF{
	int64_t n_decided=0,n_tot=0,n_unknown=0;
	std::mt19937 mt;
	void expand(TM &tm,int64_t min_unused_state,int64_t min_unused_char,int64_t rest_trans,int64_t s,int64_t in){
		#pragma omp atomic
		++n_decided;
		if(rest_trans==1)return;
		//if(rest_trans==5&&mt()%293!=2)return;
		auto &tr=tm.trans[s][in];
		auto tr0=tr;
		for(int64_t s0=0;s0<=min_unused_state&&s0<N_STATE;++s0){
			for(int64_t out=0;out<=min_unused_char&&out<N_CHAR;++out){
				for(int64_t dir:{-1,1}){
					tr.nxt=s0;
					tr.dir=dir;
					tr.out=out;
					dfs0(tm,min_unused_state+(s0==min_unused_state),min_unused_char+(out==min_unused_char),rest_trans-1);
				}
			}
		}
		tr=tr0;
	}
	decltype(std::chrono::high_resolution_clock::now()) t0=std::chrono::high_resolution_clock::now();
	void show_status(){
		auto t1=std::chrono::high_resolution_clock::now();
		auto dt=std::chrono::duration_cast<std::chrono::seconds>(t1-t0).count();
		std::cout<<n_unknown<<"/"<<n_tot<<"    "<<dt<<"s    "<<(n_unknown*1./n_tot)<<"    "<<n_tot*1./dt<<" TM/s"<<"         \r";
		std::cout.flush();
	};
	bool is_bfs=0;
	std::vector<std::tuple<TM,int64_t,int64_t,int64_t>> todo;
	void dfs0(TM &tm,int64_t min_unused_state,int64_t min_unused_char,int64_t rest_trans){
		if(is_bfs){
			todo.emplace_back(tm,min_unused_state,min_unused_char,rest_trans);
			return;
		}
		dfs(tm,min_unused_state,min_unused_char,rest_trans);
	}
	static int64_t run_ng_plain(const TM &tm,int64_t maxT,size_t ng_n){
		if constexpr(N_CHAR!=2||N_STATE>8){
			return run_ng(tm,0,0,maxT,ng_n);
		}
		// Specialized exact MitM_CTL::Decider<NG> for len=1, len_h=0, tH=0, pos_mod=1, ng_n<=3 (at most 16 states)
		const uint8_t ng_lim=(uint8_t)(1u<<ng_n);
		const uint8_t ng_mask=(uint8_t)(ng_lim-1);
		struct SmallDFA{
			uint8_t id_of_code[16];
			uint8_t code_of_id[16];
			uint8_t push_tab[16][2];
			uint8_t pop_cnt[16];
			uint8_t pop_edges[16][32]; // (ls<<1)|b
			uint16_t upds[40];         // (ls0<<8)|(ls<<1)|b
			uint8_t n_states,upds_sz;
			int32_t n_upd;
			inline void init(uint8_t ng_lim,uint8_t ng_mask){
				memset(id_of_code,0,sizeof(id_of_code));
				memset(push_tab,0,sizeof(push_tab));
				memset(pop_cnt,0,sizeof(pop_cnt));
				n_states=1;
				id_of_code[1]=1;
				code_of_id[1]=1;
				upds_sz=0;
				n_upd=0;
				uint8_t id1=1;
				do id1=push(0,id1,ng_lim,ng_mask); while(id1!=1);
			}
			inline uint8_t push(uint8_t b,uint8_t ls,uint8_t ng_lim,uint8_t ng_mask){
				uint8_t ret=push_tab[ls][b];
				if(!ret){
					uint8_t code=code_of_id[ls];
					if(!(code==1&&b==0)){
						if(code<ng_lim)code=(uint8_t)((code<<1)|b);
						else code=(uint8_t)(ng_lim|((code<<1)&ng_mask)|b);
					}
					uint8_t ls0=id_of_code[code];
					if(!ls0){
						ls0=++n_states;
						id_of_code[code]=ls0;
						code_of_id[ls0]=code;
					}
					push_tab[ls][b]=ls0;
					uint8_t edge=(uint8_t)((ls<<1)|b);
					pop_edges[ls0][pop_cnt[ls0]++]=edge;
					upds[upds_sz++]=(uint16_t)(((uint16_t)ls0<<8)|edge);
					++n_upd;
					return ls0;
				}
				return ret;
			}
		}lset,rset;
		lset.init(ng_lim,ng_mask);
		rset.init(ng_lim,ng_mask);
		uint64_t inq_lo[2][16]={};
		uint64_t inq_hi[2][16]={};
		uint32_t q[512]; // (l<<16)|(r<<8)|(s<<1)|(sgn==1)
		int32_t q_sz=0;
		auto push_q=[&](uint8_t l,uint8_t r,uint8_t s,int8_t sgn){
			int side=(sgn==1?1:0);
			uint32_t bit=((uint32_t)l<<3)|s;
			uint64_t &word=(bit<64)?inq_lo[side][r]:inq_hi[side][r];
			uint64_t mask=1ULL<<(bit&63);
			if(!(word&mask)){
				word|=mask;
				q[q_sz++]=((uint32_t)l<<16)|((uint32_t)r<<8)|((uint32_t)s<<1)|(uint32_t)side;
			}
		};
		push_q(1,1,0,1);
		int64_t used_T=0;
		for(;;){
			if(q_sz>0){
				uint32_t item=q[--q_sz];
				uint8_t l=(uint8_t)(item>>16);
				uint8_t r=(uint8_t)(item>>8);
				uint8_t s=(uint8_t)((item>>1)&7);
				int8_t sgn=(item&1)?1:-1;
				SmallDFA &d=(sgn==1?rset:lset);
				SmallDFA &o=(sgn==1?lset:rset);
				uint8_t cnt=d.pop_cnt[r];
				for(uint8_t i=0;i<cnt;++i){
					uint8_t edge=d.pop_edges[r][i];
					uint8_t b=edge&1;
					uint8_t r1=edge>>1;
					const Trans &tr=tm.trans[s][b];
					if(tr.nxt==-1)return 0;
					if(tr.dir*sgn<0){
						uint8_t nl=d.push((uint8_t)tr.out,r1,ng_lim,ng_mask);
						push_q(nl,l,(uint8_t)tr.nxt,(int8_t)(-sgn));
					}else{
						uint8_t nl=o.push((uint8_t)tr.out,l,ng_lim,ng_mask);
						push_q(nl,r1,(uint8_t)tr.nxt,sgn);
					}
				}
			}else if(lset.upds_sz>0){
				uint16_t u=lset.upds[--lset.upds_sz];
				uint8_t id0=(uint8_t)(u>>8);
				uint8_t b=(uint8_t)(u&1);
				uint8_t r1=(uint8_t)((u>>1)&0x7F);
				uint64_t m0=inq_lo[0][id0],m1=inq_hi[0][id0];
				while(m0){
					uint32_t bit=(uint32_t)__builtin_ctzll(m0);
					m0&=m0-1;
					uint8_t l=(uint8_t)(bit>>3),s=(uint8_t)(bit&7);
					const Trans &tr=tm.trans[s][b];
					if(tr.nxt==-1)return 0;
					if(tr.dir>0){ // tr.dir * (-1) < 0
						uint8_t nl=lset.push((uint8_t)tr.out,r1,ng_lim,ng_mask);
						push_q(nl,l,(uint8_t)tr.nxt,1);
					}else{
						uint8_t nl=rset.push((uint8_t)tr.out,l,ng_lim,ng_mask);
						push_q(nl,r1,(uint8_t)tr.nxt,-1);
					}
				}
				while(m1){
					uint32_t bit=64u+(uint32_t)__builtin_ctzll(m1);
					m1&=m1-1;
					uint8_t l=(uint8_t)(bit>>3),s=(uint8_t)(bit&7);
					const Trans &tr=tm.trans[s][b];
					if(tr.nxt==-1)return 0;
					if(tr.dir>0){
						uint8_t nl=lset.push((uint8_t)tr.out,r1,ng_lim,ng_mask);
						push_q(nl,l,(uint8_t)tr.nxt,1);
					}else{
						uint8_t nl=rset.push((uint8_t)tr.out,l,ng_lim,ng_mask);
						push_q(nl,r1,(uint8_t)tr.nxt,-1);
					}
				}
			}else if(rset.upds_sz>0){
				uint16_t u=rset.upds[--rset.upds_sz];
				uint8_t id0=(uint8_t)(u>>8);
				uint8_t b=(uint8_t)(u&1);
				uint8_t r1=(uint8_t)((u>>1)&0x7F);
				uint64_t m0=inq_lo[1][id0],m1=inq_hi[1][id0];
				while(m0){
					uint32_t bit=(uint32_t)__builtin_ctzll(m0);
					m0&=m0-1;
					uint8_t l=(uint8_t)(bit>>3),s=(uint8_t)(bit&7);
					const Trans &tr=tm.trans[s][b];
					if(tr.nxt==-1)return 0;
					if(tr.dir<0){ // tr.dir * (+1) < 0
						uint8_t nl=rset.push((uint8_t)tr.out,r1,ng_lim,ng_mask);
						push_q(nl,l,(uint8_t)tr.nxt,-1);
					}else{
						uint8_t nl=lset.push((uint8_t)tr.out,l,ng_lim,ng_mask);
						push_q(nl,r1,(uint8_t)tr.nxt,1);
					}
				}
				while(m1){
					uint32_t bit=64u+(uint32_t)__builtin_ctzll(m1);
					m1&=m1-1;
					uint8_t l=(uint8_t)(bit>>3),s=(uint8_t)(bit&7);
					const Trans &tr=tm.trans[s][b];
					if(tr.nxt==-1)return 0;
					if(tr.dir<0){
						uint8_t nl=rset.push((uint8_t)tr.out,r1,ng_lim,ng_mask);
						push_q(nl,l,(uint8_t)tr.nxt,-1);
					}else{
						uint8_t nl=lset.push((uint8_t)tr.out,l,ng_lim,ng_mask);
						push_q(nl,r1,(uint8_t)tr.nxt,1);
					}
				}
			}else{
				return 2;
			}
			++used_T;
			if(used_T+q_sz+lset.upds_sz+rset.upds_sz>=maxT)return 0;
			if(lset.n_upd+rset.n_upd>=maxT)return 0;
		}
	}
	static int64_t run_ng(const TM &tm,int64_t lruh,int64_t h,int64_t maxT,size_t ng_n){
		if constexpr(N_CHAR==2&&log2_N_STATE==3){
			if(lruh>=1&&lruh<=14&&ng_n>=1&&ng_n<=3&&maxT<65535){
				thread_local Decider64 d64;
				return d64.decide(tm,lruh,h,maxT,ng_n);
			}
		}
		ChrTransform::init();
		ChrTransform::len_h=lruh;
		ChrTransform::len_h_no_lru=h;
		MitM_CTL::NG::init();
		MitM_CTL::NG::NG_n=ng_n;
		thread_local MitM_CTL::Decider<MitM_CTL::NG> f;
		f.len=1;
		DH_Tape tape0;
		return f.decide(tm,maxT,tape0);
	}
	static int64_t run_cps_lru(const TM &tm,const DH_Tape &tape_sim,int64_t maxT,int64_t lruh,int64_t h,int64_t th,size_t n){
		if(tape_sim.s==-1)return 0;
		ChrTransform::init();
		MitM_CTL::CPS_LRU::init();
		MitM_CTL::CPS_LRU::len_h=lruh;
		MitM_CTL::CPS_LRU::len_h_no_lru=h;
		MitM_CTL::CPS_LRU::len_h_tail=th;
		thread_local MitM_CTL::Decider<MitM_CTL::CPS_LRU> f;
		f.len=n;
		return f.decide(tm,maxT,tape_sim);
	}
	static int64_t run_rwl_mod(const TM &tm,const DH_Tape &tape_sim,int64_t maxT,int64_t h,int64_t mod,size_t n){
		if(tape_sim.s==-1)return 0;
		ChrTransform::init();
		MitM_CTL::RWL_mod::init();
		MitM_CTL::RWL_mod::len_h=h;
		MitM_CTL::RWL_mod::mod=mod;
		thread_local MitM_CTL::Decider<MitM_CTL::RWL_mod> f;
		f.len=n;
		return f.decide(tm,maxT,tape_sim);
	}
	void dfs(TM &tm,int64_t min_unused_state,int64_t min_unused_char,int64_t rest_trans){
		if(app_stopped)return;
		FastTape ht;
		if(ht.run_first_50(tm)){
			expand(tm,min_unused_state,min_unused_char,rest_trans,ht.last_s,ht.last_in);
			goto o;
		}
		if(ht.check_tc(50)){
			goto o_nonhalt;
		}
		if(ht.run_next_250(tm)){
			expand(tm,min_unused_state,min_unused_char,rest_trans,ht.last_s,ht.last_in);
			goto o;
		}
		if(ht.check_tc(300)){
			goto o_nonhalt;
		}
		MitM_CTL::CTL_skip_verify=1;
		if(run_ng_plain(tm,100,1)==2)goto o_nonhalt;
		if(run_ng_plain(tm,200,2)==2)goto o_nonhalt;
		if(run_ng_plain(tm,300,3)==2)goto o_nonhalt;
		if(run_ng(tm,6,3,3000,3)==2)goto o_nonhalt;
		{
			LongTape lt(ht);
			if(lt.run_steps<true>(tm,701)){
				expand(tm,min_unused_state,min_unused_char,rest_trans,lt.last_s,lt.last_in);
				goto o;
			}
			if(lt.run_steps<true>(tm,1999)){
				expand(tm,min_unused_state,min_unused_char,rest_trans,lt.last_s,lt.last_in);
				goto o;
			}
			if(lt.check_tc(3000)){
				goto o_nonhalt;
			}
			if(run_ng(tm,14,8,1000,2)==2)goto o_nonhalt;
			if(run_ng(tm,4,2,3000,1)==2)goto o_nonhalt;
			DH_Tape tape_1001;
			tape_1001.check_halt(tm,1001);
			if(run_cps_lru(tm,tape_1001,1000,4,1,0,6)==2)goto o_nonhalt;
			if(run_ng(tm,12,12,3000,3)==2)goto o_nonhalt;
			if(run_rwl_mod(tm,tape_1001,3000,4,2,8)==2)goto o_nonhalt;
			if(lt.run_steps<true>(tm,27000)){
				expand(tm,min_unused_state,min_unused_char,rest_trans,lt.last_s,lt.last_in);
				goto o;
			}
			if(lt.check_tc(30000)){
				goto o_nonhalt;
			}
			if(run_rwl_mod(tm,tape_1001,10000,3,2,1)==2)goto o_nonhalt;
			if(run_cps_lru(tm,tape_1001,3000,12,1,1,2)==2)goto o_nonhalt;
			if(run_rwl_mod(tm,tape_1001,3000,6,3,2)==2)goto o_nonhalt;
			if(run_ng(tm,0,0,10000,5)==2)goto o_nonhalt;
			if(run_cps_lru(tm,tape_1001,3000,4,0,1,4)==2)goto o_nonhalt;
			if(run_ng(tm,6,6,3000,1)==2)goto o_nonhalt;
			if(run_ng(tm,16,12,10000,5)==2)goto o_nonhalt;
			if(run_cps_lru(tm,tape_1001,3000,16,0,2,3)==2)goto o_nonhalt;
			if(run_ng(tm,5,1,10000,2)==2)goto o_nonhalt;
			if(lt.run_steps<false>(tm,270000)){
				expand(tm,min_unused_state,min_unused_char,rest_trans,lt.last_s,lt.last_in);
				goto o;
			}
		}
		
		o_fail:
		
		#pragma omp critical
		{
			++n_unknown;
			merr<<tm<<'\n';
		}
		/*if(n_unknown%100==0){
			std::cout<<tm<<"                                "<<'\n';
			show_status();
		}*/
		goto o;
		o_nonhalt:
		#pragma omp atomic
		++n_decided;
		o:;
		/*DH_Tape t;
		if(auto o=t.check_halt(tm,20)){
			auto [T,s,in]=*o;
			expand(tm,min_unused_state,s,in);
		}else{
			//std::cout<<tm<<'\n';
		}*/
		#pragma omp critical
		{
			++n_tot;
			if(n_tot%1000000==0){
				//std::cout<<tm<<'\n';
				show_status();
				/*if(n_tot==64000000){
					std::cout<<'\n';
					merr.flush();
					exit(0);
				}*/
			}
		}
	}
};

int main_1(int64_t pos){
	constexpr int64_t N=10;
	std::cout<<"task id:  "<<pos<<std::endl;
	assert(0<=pos&&pos<1000000);
	TNF tnf;
	tnf.is_bfs=1;
	{
		TM tm;
		tm.trans[0][0].nxt=1;
		tm.trans[0][0].dir=1;
		tm.trans[0][0].out=1;
		tnf.todo.emplace_back(tm,2,2,N_STATE*N_CHAR-1);
	}
	for(int64_t i=0;i<4+6;++i){
		/*{
			int msz=1e4+1700;
			auto &todo=tnf.todo;
			std::random_shuffle(todo.begin(),todo.end());
			if(todo.size()>msz)todo.resize(msz);
			if(i>=8)break;
		}*/
		if(i>=4){
			int p=pos%N;
			auto todo=std::move(tnf.todo);
			for(int64_t i=p;i<todo.size();i+=N){
				tnf.todo.emplace_back(todo.at(i));
			}
			pos/=N;
		}
		
		auto todo=std::move(tnf.todo);
		for(auto [tm0,a,b,c]:todo){
			//if(i<=1)std::cout<<tm0<<'\n';
			tnf.dfs(tm0,a,b,c);
		}
		std::cout<<i<<":  "<<tnf.todo.size()<<std::endl;
	}
	tnf.is_bfs=0;
	{
		auto todo=std::move(tnf.todo);
		auto sz=todo.size();
		std::cout<<"root num: "<<sz<<std::endl;
		//#pragma omp parallel for schedule(dynamic) num_threads(8)
		for(size_t i=0;i<sz;++i){
			auto [tm0,a,b,c]=todo[i];
			//std::cout<<"root: "<<tm0<<std::endl;
			tnf.dfs(tm0,a,b,c);
		}
	}
	tnf.show_status();
	std::cout<<std::endl;
	return 0;
}

int main_enum(int argc,char **argv){
	assert(argc>=2);
	int64_t l=atoi(argv[1]),r=l;
	if(argc>=3){
		r=atoi(argv[2]);
		assert(argc==3);
	}
	std::cout<<"task id:  "<<l<<".."<<r<<std::endl;
	for(int64_t i=l;i<=r;++i){
		log_stream=std::ofstream(std::string("holdouts_")+std::to_string(i)+".txt");
		main_1(i);
		if(app_stopped){
			merr<<"stopped\n";
			break;
		}
	}
	std::cout<<"finished"<<std::endl;
	return 0;
}


int main(int argc,char **argv){
	//if(argc==1)return main_1();
	assert(argc>=2);
	signal(SIGINT,sigint_handler);
	if(!strcmp(argv[1],"enum")){
		main_enum(argc-1,argv+1);
	}else if(!strcmp(argv[1],"ucb")){
		main_ucb(argc-1,argv+1);
	}else if(!strcmp(argv[1],"verify")){
		main_verify(argc-1,argv+1,0);
	}else if(!strcmp(argv[1],"exec")){
		main_verify(argc-1,argv+1,1);
	}else{
		std::cerr<<"unknown task: "<<argv[1]<<'\n';
		assert(0);
	}
	return 0;
}
