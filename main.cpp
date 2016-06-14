#include <iostream>
#include <cstring>
#include <string.h>
#include "gtest/gtest.h"

using namespace std;

struct Representation
{	
	char * data;
	int size;
	int n;
	Representation(const char * buf, int s): size(s){ 
		data = new char[size];
		memcpy(data, buf, size);
		n = 1;
	}
	~Representation(){
		delete[] data;}
	Representation * Copy(){
		if (n == 1) return this;
		n--;
		return new Representation(data, size);}
}; 


class String{
	
struct CharReference{
        String *s;
        int i;
		CharReference(String* b, int a):s(b),i(a){}
		CharReference &operator= (char c){
		s->p= s->p->Copy();
		s->p->data[i] = c;
		return *this;
		}
		operator char(){
		return s->p->data[i];
		}
};	
public:
	CharReference operator[](int k){
		 return CharReference(this, k);
	}

	

public:
	Representation * p;

	char operator[](int i) const {
        return p->data[i];
    }

	String(){
		p = new Representation("", 0);
	}
	String(const char* a){
		p = new Representation(a, strlen(a));
	}
	String(const String& a){
		p = a.p;
		p->n++;
	}
	
	friend bool operator==(const String& a, const String& b);
	friend bool operator!=(const String& a, const String& b);
	friend String operator+(const String& a, const String& b);
	friend String& operator+=(String& a, String& b);

	void decreas_counter(){
		p->n--;
		if (p->n == 0){
			delete p;
		}
	}

	String operator=(const String& a){
		decreas_counter();
		a.p->n++;
		p=a.p;
		return *this;
	}
	
	String operator()(int i, int j){
		int new_size = j-i;
		char * b = new char [new_size+1];
		b[new_size] = '\0';
		memcpy(b, p->data+i, new_size);
		String c(b);
		delete[] b;
		return c;
	}
	~String(){decreas_counter();}
};

bool operator==(const String& a, const String& b){
		if (a.p->size != b.p->size) {
			return false;
		}
	
        for (int i = 0; i < b.p->size; ++i) {
            if (a.p->data[i] != b.p->data[i])
                return false;
		}
		return true;
	}

bool operator!=(const String& a, const String& b){
		return !(operator== (a,b));
	}

String operator+(const String& a, const String& s){
		int new_size = s.p->size + a.p->size;
	    char * b = new char [new_size];
		memcpy(b, a.p->data, a.p->size);
		memcpy(b+a.p->size, s.p->data, s.p->size);
		b[new_size] = '\0';
		String c(b);
		return c;
	}

String& operator+=(String& a, String& b){
		int new_size = b.p->size + a.p->size;
		char * c = new char[new_size];
		memcpy(c, a.p->data, a.p->size);
		memcpy(c+a.p->size, b.p->data, b.p->size);
		a.decreas_counter();
		a.p = new Representation(c, new_size);
		return a;
}

TEST(String, EQ){
	String a("abg");
	String b("abg");
    ASSERT_TRUE(String("asd") == String("asd"));
	ASSERT_TRUE(a == b);
};
TEST(String, noEQ){
	String a("arhg");
	String b("abg");
    ASSERT_TRUE(String("asd") != String("ard"));
	ASSERT_TRUE(a != b);
};


TEST(String, Add_1){
	String a("bpk");
	String b("gdp");
    ASSERT_TRUE(( String("dsa")+String("aba")) == String("dsaaba"));
	ASSERT_TRUE((a + b) == String("bpkgdp"));
	String c = a + b;
	ASSERT_TRUE(c == String("bpkgdp"));
	String d("K");
	d.p = c.p->Copy();
	ASSERT_TRUE( d ==  c);
};
TEST(String, Add_2){
	String a("cops");
	String b("abab");
    ASSERT_TRUE((String("aba") += String("dsa")) == String("abadsa"));
	a+=b;
	ASSERT_TRUE( a == String("copsabab"));
};


TEST(String, Char){
    String a("bad");
    ASSERT_TRUE(a[0] == 'b');
    ASSERT_TRUE(a[1] == 'a');
    ASSERT_TRUE(a[2] == 'd');
};

TEST(String, Copy){
    String a("gec");
	String b("ijqwen");
	a.p = b.p->Copy();
    ASSERT_TRUE(a == b);
};

TEST(String, Change){
    String a("bads");
    a[0] = 'a';
    ASSERT_TRUE(a == String("aads"));
	String b("q");
	b.p = a.p->Copy();
	a[1] = 'd';
	ASSERT_EQ(b , String("adds"));
};

TEST(String, cut){
	String a("badsd");
	String b = a(1,3);
	String d = a(1,2);
	String m  = a(1,1);
	String c;
	c.p = b.p->Copy();
	ASSERT_EQ( c , String("ad"));
	ASSERT_EQ( b , String("ad"));
	ASSERT_EQ( d , String("a"));
	ASSERT_EQ( m , String(""));

}

TEST(String, assign){
	String a("badsd");
	String b("edcv");
	a = b; 
	b[1] = 'q';
	ASSERT_EQ( a  , String("edcv"));
}

int main(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}