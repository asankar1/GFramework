#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <GSerializer.h>
#include <GVariant.h>



#ifdef VARIANT_DYNAMIC_LIBRARY
#ifdef DLL_EXPORT
#define LIBRARY_API __declspec( dllexport )
#else
#define LIBRARY_API __declspec( dllimport )
#endif
#else
#define LIBRARY_API
#endif

namespace GFramework
{
	class Object;

	void register_lua_script_functions(lua_State *L, std::vector<luaL_Reg>& GPropertiesList);

	class LIBRARY_API GPropertyInterface
	{
	public:
		virtual void set(GVariant& _value) = 0;
		virtual GVariant get() = 0;
		virtual std::ostream& writeBinaryValue(std::ostream& os) const = 0;
		virtual std::istream& readBinaryValue(std::istream& is) = 0;
		virtual std::ostream& writeASCIIValue(std::ostream& os) const = 0;
		virtual std::istream& readASCIIValue(std::istream& is) = 0;
	protected:
		GPropertyInterface() {}
	};

	template <typename T>
	class GScalarProperty : public GPropertyInterface
	{
	public:
		GScalarProperty();

		virtual void set(GVariant& _value);

		void setValue(T _value);

		virtual GVariant get();

		const T& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);


	private:
		T value;
	};

	class GStringProperty : public GPropertyInterface
	{
	public:
		GStringProperty();

		virtual void set(GVariant& _value);

		void setValue(const std::string& _value);

		virtual GVariant get();

		const std::string& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		std::string value;
	};

	template <typename T>
	class GGlmProperty : public GPropertyInterface
	{
	public:
		GGlmProperty();

		virtual void set(GVariant& _value);

		void setValue(T _value);

		virtual GVariant get();

		const T& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		T value;
	};

	template<typename T>
	class LIBRARY_API GNodePointerProperty : public GPropertyInterface
	{
	public:
		GNodePointerProperty(Object* _owner=nullptr) : owner(_owner){

		}

		~GNodePointerProperty(){
			if (value != nullptr) {
				value->removeObserver(owner);
			}
		}

		virtual void set(GVariant& _value) {
			setValue(boost::any_cast<T>(_value));
		}

		void setValue(T& _value) {
			assert(owner);
			if (value != nullptr) {
				value->removeObserver(owner);
			}
			value = _value;
			if (value != nullptr) {
				value->addObserver(owner);
			}
		}

		virtual GVariant get() {
			return value;
		}

		const T& getValue() const {
			return value;
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os) const {
			if (value)
			{
				os << value->getObjectId();
			}
			else
			{
				os << (unsigned int)0;
			}
			return os; 
		}
		
		virtual std::istream& readBinaryValue(std::istream& is) {
			unsigned int parent_obj_id = 0;
			is >> parent_obj_id;
			if (parent_obj_id)
			{			
				GDeserializer::addReferenceSeeker(parent_obj_id, &value);
			}
			return is; 
		}

		virtual std::ostream& writeASCIIValue(std::ostream& os) const {
			if (value)
			{
				os << value->getObjectId();
			}
			else
			{
				os << (unsigned int)0;
			}
			return os; 
		}
		virtual std::istream& readASCIIValue(std::istream& is) {
			unsigned int parent_obj_id = 0;
			is >> parent_obj_id;
			if (parent_obj_id)
			{
				GDeserializer::addReferenceSeeker(parent_obj_id, &value);
			}
			return is; 
		}

	private:
		T value;
		Object* owner;
	};

	template<typename T>
	class LIBRARY_API GProperty : public GPropertyInterface
	{
	public:

		virtual void set(GVariant& _value) {
			value = boost::any_cast<T>(_value);
		}

		void setValue(T _value) {
			value = _value;
		}

		virtual GVariant get() {
			return value;
		}

		const T& getValue() const {
			return value;
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os) const { return os; }
		virtual std::istream& readBinaryValue(std::istream& is) { return is; }
		virtual std::ostream& writeASCIIValue(std::ostream& os) const { return os; }
		virtual std::istream& readASCIIValue(std::istream& is) { return is; }

	private:
		T value;
	};

	typedef GGlmProperty<glm::vec2> GVec2Property;
	typedef GGlmProperty<glm::vec3> GVec3Property;
	typedef GScalarProperty<bool> GBoolProperty;
	typedef GScalarProperty<char> GCharProperty;
	typedef GScalarProperty<unsigned char> GUcharProperty;
	typedef GScalarProperty<short> GPShortroperty;
	typedef GScalarProperty<unsigned short> GUshortProperty;
	typedef GScalarProperty<int> GIntProperty;
	typedef GScalarProperty<unsigned int> GUintProperty;
	typedef GScalarProperty<float> GFloatProperty;
	typedef GScalarProperty<double> GDoubleProperty;

}
