#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <glm/gtc/type_ptr.hpp>
#include <GVariant/GTypes.h>
//#include <GSerializer.h>
#include <GVariant/GVariant.h>

namespace GFramework
{
	class GObject;

	void register_lua_script_functions(lua_State *L, std::vector<luaL_Reg>& GPropertiesList);

	class GFRAMEWORK_API GPropertyInterface
	{
	public:
		virtual void set(GVariant& _value) = 0;
		virtual GVariant get() const = 0;
		virtual std::ostream& writeBinaryValue(std::ostream& os) const = 0;
		virtual std::istream& readBinaryValue(std::istream& is) = 0;
		virtual std::ostream& writeASCIIValue(std::ostream& os) const = 0;
		virtual std::istream& readASCIIValue(std::istream& is) = 0;
	protected:
		GPropertyInterface() {}
	};

	template <typename T>
	class GFRAMEWORK_API GArithmeticProperty : public GPropertyInterface
	{
		static_assert(std::is_arithmetic<T>::value, "Template argument T to GArithmeticProperty must be an arithmetic type!");
	public:
		GArithmeticProperty(T v = std::numeric_limits<T>::min());

		virtual void set(GVariant& _value);

		void setValue(T _value);

		virtual GVariant get() const;

		const T& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		T value;
	};

	class GFRAMEWORK_API GStringProperty : public GPropertyInterface
	{
	public:
		GStringProperty();

		virtual void set(GVariant& _value);

		void setValue(const std::string& _value);

		virtual GVariant get() const;

		const std::string& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		std::string value;
	};

	template <typename T>
	class GFRAMEWORK_API GGlmProperty : public GPropertyInterface
	{
	public:
		GGlmProperty(T v = T());

		virtual void set(GVariant& _value);

		void setValue(T _value);

		virtual GVariant get() const;

		const T& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		T value;
	};

	class  GPointerPropertyInterface : public GPropertyInterface
	{
	public:
		virtual void subjectDeleted() = 0;
                GPointerPropertyInterface(const GPointerPropertyInterface&) = delete;
                GPointerPropertyInterface& operator=(const GPointerPropertyInterface&) = delete;

	protected:
		GPointerPropertyInterface() {}
	};

	template<typename T>
	class  GPointerProperty : public GPointerPropertyInterface
	{
		//static_assert(std::is_pointer<T>::value, "Template argument to GPointerProperty must be a pointer!");
		//TODO: fix //static_assert(std::is_base_of<GObject, T>::value, "Template argument T to GPointerProperty must be derived from Object class directly or indirectly!");
	public:
		GPointerProperty(T* v = nullptr) : value(v){

		}

		virtual ~GPointerProperty() {
			if (value != nullptr) {
				value->unSubscribeDeletionNotification(this);
			}
		}

		virtual void set(GVariant& _value) {
			setValue(GVariant::cast<T*>(_value));
		}

		void setValue(T* _value) {
			if (value != nullptr) {
				value->unSubscribeDeletionNotification(this);
			}
			value = _value;
			if (value != nullptr) {
				value->subscribeDeletionNotification(this);
			}
		}

		virtual GVariant get() const {
			return GVariant::create <T*>(value);
		}

		T* getValue() const {
			return value;
		}

		virtual void subjectDeleted() {
			value = nullptr;
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os) const {
			if (value)
			{
				os << value->getObjectId();
			}
			else
			{
				os << (uint32)0;
			}
			return os;
		}

		virtual std::istream& readBinaryValue(std::istream& is) {
			uint32 pointer_obj_id = 0;
			is >> pointer_obj_id;
			if (pointer_obj_id)
			{
				//TODO: Fix: GDeserializer::addReferenceSeeker(pointer_obj_id, value);
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
				os << (uint32)0;
			}
			return os;
		}
		virtual std::istream& readASCIIValue(std::istream& is) {
			uint32 pointer_obj_id = 0;
			is >> pointer_obj_id;
			if (pointer_obj_id)
			{
				//TODO: Fix: GDeserializer::addReferenceSeeker(pointer_obj_id, value);
			}
			return is;
		}

	private:
		T* value;
	};

	/*template<typename T>
	class  GProperty : public GPropertyInterface
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
	};*/

	typedef GGlmProperty<glm::vec2> GVec2Property;
	typedef GGlmProperty<glm::vec3> GVec3Property;
	typedef GGlmProperty<glm::vec4> GVec4Property;
	typedef GGlmProperty<glm::mat2> GMat2Property;
	typedef GGlmProperty<glm::mat3> GMat3Property;
	typedef GGlmProperty<glm::mat4> GMat4Property;
	typedef GArithmeticProperty<bool> GBoolProperty;
	typedef GArithmeticProperty<char> GCharProperty;
	typedef GArithmeticProperty<int8> GInt8Property;
	typedef GArithmeticProperty<uint8> GUint8Property;
	typedef GArithmeticProperty<int16> GInt16troperty;
	typedef GArithmeticProperty<uint16> GUint16Property;
	typedef GArithmeticProperty<int32> GInt32Property;
	typedef GArithmeticProperty<uint32> GUint32Property;
	typedef GArithmeticProperty<int64> GInt64Property;
	typedef GArithmeticProperty<uint64> GUint64Property;
	typedef GArithmeticProperty<float> GFloatProperty;
	typedef GArithmeticProperty<double> GDoubleProperty;
	typedef GPointerProperty<GObject> GObjectPointerProperty;
	//typedef GPointerProperty<Node> GNodePointerProperty;
}
