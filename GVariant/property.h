#pragma once
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <Object.h>
#include <GVariant.h>
#include <GSerializer.h>

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
		GScalarProperty() {}

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

		std::ostream& writeASCIIValue(std::ostream& os) const {
			os << value << " ";
			return os;
		}

		std::istream& readASCIIValue(std::istream& is) {
			is >> value;
			return is;
		}

		std::ostream& writeBinaryValue(std::ostream& os) const {
			os.write(reinterpret_cast<const char*>(&value), sizeof(T));
			return os;
		}

		std::istream& readBinaryValue(std::istream& is) {
			is.read(reinterpret_cast<char*>(&value), sizeof(T));
			return is;
		}

	private:
		T value;
	};

	class GStringProperty : public GPropertyInterface
	{
	public:
		GStringProperty() {}

		virtual void set(GVariant& _value) {
			value = boost::any_cast<std::string>(_value);
		}

		void setValue(const std::string& _value) {
			value = _value;
		}

		virtual GVariant get() {
			return value;
		}

		const std::string& getValue() const {
			return value;
		}

		std::ostream& writeASCIIValue(std::ostream& os) const {
			os << value.length() << " " << value << " ";
			return os;
		}

		std::istream& readASCIIValue(std::istream& is) {
			size_t count;
			is >> count;
			std::string str;
			while (value.length() < count)
			{
				is >> str;
				value.append(str);
			}
			
			return is;
		}

		std::ostream& writeBinaryValue(std::ostream& os) const {
			size_t size = value.length()+1;
			os.write(reinterpret_cast<char*>(&size), sizeof(size_t));
			os.write(value.c_str(), size);
			return os;
		}

		std::istream& readBinaryValue(std::istream& is) {
			size_t count;
			is.read(reinterpret_cast<char*>(&count), sizeof(size_t));
			char* str = new char[count];
			is.read(str, count);
			value = std::string(str);
			delete[] str;
			return is;
		}

	private:
		std::string value;
	};

	template <typename T>
	class GGlmProperty : public GPropertyInterface
	{
	public:
		GGlmProperty() {}

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

		std::ostream& writeASCIIValue(std::ostream& os) const {
			unsigned int size = value.length();
			const float* pointer = glm::value_ptr(value);
			for (unsigned int i= 0; i < size; i++)
			{
				os << pointer[i] << " ";
			}
			return os;
		}

		std::istream& readASCIIValue(std::istream& is) {
			unsigned int size = value.length();
			float* pointer = glm::value_ptr(value);
			for (unsigned int i= 0; i < size; i++)
			{
				is >> pointer[i];
			}
			return is;
		}

		std::ostream& writeBinaryValue(std::ostream& os) const {
			unsigned int size = value.length();
			const float* pointer = glm::value_ptr(value);
			os.write(reinterpret_cast<const char*>(pointer), size * sizeof(float));
			return os;
		}

		std::istream& readBinaryValue(std::istream& is) {
			unsigned int size = value.length();
			float* pointer = glm::value_ptr(value);
			is.read(reinterpret_cast<char*>(pointer), size * sizeof(float));
			return is;
		}

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



}
