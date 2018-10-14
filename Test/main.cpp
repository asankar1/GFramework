#include <iostream>
#include <conio.h>
#include <typeindex>
#include <vector>
#include <map>
#include "gvariant_test.h"
#include "greflection_test.h"
#include "gserialization_test.h"
#include "gscript_test.h"
using namespace std;
#define ROUGH 0
void rough();
void main()
{
	
#if ROUGH
	rough();
#else
	run_variant_testcases();
	run_reflection_testcases();
	//run_serialization_testcases();
	//run_script_testcases();
#endif
	cout << "\nPress any key to exit...";
	_getch();
}

class NullType {};
template <class T, class U>
struct TypeList {
	typedef T Head;
	typedef U Tail;
};
typedef TypeList< int, TypeList<float, TypeList<char, NullType> > > MyTypeList;

template <typename R>
void createCmd(const std::string& name, R(*fffp)())
{
	typedef void(*fffp_t)();
	cout << typeid(R).name() ;

	return;// cmdCreator<fffp_t>::createCmd(name, fffp);
}

template <typename R, typename T>
void createCmd(const std::string& name, R(*fffp)(T))
{
	typedef void(*fffp_t)(T);
	cout << typeid(R).name() << "," << typeid(T).name();

	return;// cmdCreator<fffp_t>::createCmd(name, fffp);
}

template <typename R, typename T1, typename T2>
void createCmd(const std::string& name, R(*fffp)(T1, T2))
{
	typedef R(*fffp_t)(T1, T2);
	cout << typeid(R).name() << "," << typeid(T1).name() << "," << typeid(T2).name();
}

void foo()
{
	return ;
}

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost\any.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <tuple>
#include <utility>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
using namespace std;

template <typename FuncType>
using Arity = boost::function_types::function_arity<FuncType>;

template <typename FuncType>
using ResultType = typename boost::function_types::result_type<FuncType>::type;

template <typename FuncType, size_t ArgIndex>
using ArgType = typename boost::mpl::at_c<boost::function_types::parameter_types<FuncType>, ArgIndex>::type;
 
void func(int, char, double) {}

template <typename Func, typename IndexSeq>
struct ArgPrintHelper;

template <typename Func, size_t... Inds>
struct ArgPrintHelper<Func, integer_sequence<size_t, Inds...> >
{
	static void doit()
	{
		string typeNames[] = { typeid(ResultType<Func>).name(), typeid(ArgType<Func, Inds>).name()... };
		for (auto const& name : typeNames)
			cout << name << " ";
		cout << endl;
	}
};

template <typename Func>
void ArgPrinter(Func f)
{
	//ArgPrintHelper<Func, make_index_sequence<Arity<Func>::value> >::doit();
	ArgPrintHelper<Func, make_index_sequence<Arity<Func>::value> >::doit();
}

int doo(int i,  int j, int k)
{
	cout << i << j << k;
	return i+j+k;
}
#include <boost\variant.hpp>
#include <boost\function.hpp>
#include <array>
template <typename Func>
void pr(Func f)
{
	cout << "return type: " << typeid(boost::function_types::result_type<Func>::type).name() << endl;
	cout << "args count "<< boost::function_types::function_arity<Func>::value << endl;
}

#if ROUGH
void foo(unsigned int i, float f, double* dp)
{
	cout << i << "," << f << "," << *dp << endl;
}

void bar(unsigned int i, float f, double d)
{
	cout << i << "," << f << "," << d << endl;
}

template<typename TARGET_TYPE>
class my_visitor : public boost::static_visitor<TARGET_TYPE>
{
public:
	template<class VARIANT_TYPE>
	TARGET_TYPE operator()(VARIANT_TYPE v) const
	{
		return v;
	}
};

template <typename... Types>
struct my_variant : public boost::variant<Types...>
{
	using boost::variant<Types...>::variant;
	template<typename TARGET_TYPE>
	operator TARGET_TYPE() const
	{
		return boost::get<TARGET_TYPE>(*this);
	}

	template<typename TARGET_TYPE>
	operator TARGET_TYPE&() const
	{
		return boost::get<TARGET_TYPE>(*this);
	}
};

struct myany : public boost::any
{
	myany() : boost::any() {

	}

	template <class ValueType>
	myany(ValueType v) : boost::any(v) {

	}
	template <class ValueType>
	myany & operator=(ValueType&& rhs)
	{
		any(static_cast<ValueType&&>(rhs)).swap(*this);
		return *this;
	}
	
	template<typename T>
	operator T&()
	{
		cout << "ref\n";
		return boost::any_cast<T&>(*this);
	}

	template<typename T>
	operator const T&()
	{
		cout << "const ref\n";
		return boost::any_cast<const T&>(*this);
	}

	template<typename T>
	operator T*()
	{
		cout << "ptr\n";
		return boost::any_cast<T*>(*this);
	}

	template<typename T>
	operator const T*()
	{
		cout << "const ptr\n";
		return boost::any_cast<const T*>(*this);
	}

	template<typename T>
	operator T() const
	{
		cout << "norm\n";
		return boost::any_cast<T>(*this);
	}
};

//typedef my_variant<int, float, double> Var;
typedef myany Var;


void boo(int f)
{
	cout << f << endl;
}

class box;

class physics
{
public:
	void addBox(shared_ptr<box>& b) {
		list.push_back(b);
	}
private:
	vector<shared_ptr<box>> list;
};

class renderer
{
public:
	void addBox(shared_ptr<box>& b) {
		list.push_back(b);
	}
private:
	vector<shared_ptr<box>> list;
};

class box
{
public:
	box() {
		cout << "Box constructed\n";
	}

	~box() {
		cout << "Box destructed\n";
	}

	void do_render(renderer& r)
	{
		//r.addBox();
	}

	void do_physics(physics& p)
	{
		//p.addBox();
	}
};

box b;
void rough()
{	
	cout << "\n\nRough work!\n";
	renderer r;
	shared_ptr<box> rbp(&b);
	r.addBox(rbp);

	{
		shared_ptr<box> pbp(&b);
		physics p;
		cout << pbp.use_count() << endl;
		p.addBox(pbp);
		cout << "in-out" << endl;
	}
	cout << "out" << endl;
}

#endif