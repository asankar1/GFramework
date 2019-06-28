#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <limits>
#include <array>
#include <GVariant/GObject.h>
#include <GVariant/GVariant.h>
#include <GVariant/GProperty.h>
#include "gproperty_test.h"

using namespace std;
using namespace GFramework;

namespace property_test {
	class Node : public GObject {
	public:
		Node() {}
		Node(int i) :id(i) {}
		virtual void initialize() {
		}
		~Node() {
			int i = 0;
		}

		virtual GMetaclass* metaclassName() {
			return GMetaNamespace::getMetaclassByType<property_test::Node>();
		}

		GPointerProperty<Node> interested_object;
		int id;
	};
}

void run_property_testcases()
{
	cout << "===========================" << endl;
	cout << "Startig GProperty testcases" << endl;
	cout << "===========================" << endl;

	{ \
		GVariant gv = GVariant::create<int>(std::numeric_limits<int>::min()); \
		GInt32Property property1; \
		GInt32Property property2; \
		property1.set(gv); \
		assert(GVariant::cast<int32>(property1.get()) == std::numeric_limits<int>::min()); \
		assert(property1.getValue() == std::numeric_limits<int>::min()); \
		property1.setValue(std::numeric_limits<int>::max()); \
		assert(GVariant::cast<int32>(property1.get()) == std::numeric_limits<int>::max()); \
		assert(property1.getValue() == std::numeric_limits<int>::max()); \
	{ \
		ostringstream os; \
		istringstream is; \
		property1.writeASCIIValue(os); \
		is.str(os.str()); \
		property2.readASCIIValue(is); \
		assert(property1.getValue() == property2.getValue()); \
	} \
	{ \
		ostringstream os; \
		istringstream is; \
		property1.writeBinaryValue(os); \
		is.str(os.str()); \
		property2.readBinaryValue(is); \
		assert(property1.getValue() == property2.getValue()); \
	} \
	}

#define CHECK_ARITHMETIC_PROPERTY(RAW_TYPE, PROPERTY_TYPE) \
	{ \
		GVariant gv = GVariant::create<RAW_TYPE>(std::numeric_limits<RAW_TYPE>::min()); \
		PROPERTY_TYPE property1; \
		PROPERTY_TYPE property2; \
		property1.set(gv); \
		assert(GVariant::cast<RAW_TYPE>(property1.get()) == std::numeric_limits<RAW_TYPE>::min()); \
		assert(property1.getValue() == std::numeric_limits<RAW_TYPE>::min()); \
		property1.setValue(std::numeric_limits<RAW_TYPE>::max()); \
		assert(GVariant::cast<RAW_TYPE>(property1.get()) == std::numeric_limits<RAW_TYPE>::max()); \
		assert(property1.getValue() == std::numeric_limits<RAW_TYPE>::max()); \
		{ \
			ostringstream os; \
			istringstream is; \
			property1.writeASCIIValue(os); \
			is.str(os.str()); \
			property2.readASCIIValue(is); \
			assert(property1.getValue() == property2.getValue()); \
		} \
		{ \
			ostringstream os; \
			istringstream is; \
			property1.writeBinaryValue(os); \
			is.str(os.str()); \
			property2.readBinaryValue(is); \
			assert(property1.getValue() == property2.getValue()); \
		} \
	}

	//Arithmetic propety types
	{
		cout << "Testing GProperty for arithmetic propety types: ";

		//bool property
		CHECK_ARITHMETIC_PROPERTY(bool, GBoolProperty);
		
		//char property
		CHECK_ARITHMETIC_PROPERTY(char, GCharProperty);

		//int8 property
		CHECK_ARITHMETIC_PROPERTY(int8, GInt8Property);

		//int16 property
		CHECK_ARITHMETIC_PROPERTY(int16, GInt16troperty);

		//int32 property
		CHECK_ARITHMETIC_PROPERTY(int32, GInt32Property);

		//int64 property
		CHECK_ARITHMETIC_PROPERTY(int64, GInt64Property);

		//float property
		CHECK_ARITHMETIC_PROPERTY(float, GFloatProperty);

		//double property
		CHECK_ARITHMETIC_PROPERTY(double, GDoubleProperty);

		cout << "Ok" << endl;
	}

#define CHECK_GLM_PROPERTY(RAW_TYPE, PROPERTY_TYPE) \
	{ \
		GVariant gv = GVariant::create<RAW_TYPE>(RAW_TYPE(std::numeric_limits<float>::min())); \
		PROPERTY_TYPE property1; \
		PROPERTY_TYPE property2; \
		property1.set(gv); \
		assert(GVariant::cast<RAW_TYPE>(property1.get()) == RAW_TYPE(std::numeric_limits<float>::min())); \
		assert(property1.getValue() == RAW_TYPE(std::numeric_limits<float>::min())); \
		property1.setValue(RAW_TYPE(std::numeric_limits<float>::max())); \
		assert(GVariant::cast<RAW_TYPE>(property1.get()) == RAW_TYPE(std::numeric_limits<float>::max())); \
		assert(property1.getValue() == RAW_TYPE(std::numeric_limits<float>::max())); \
		{ \
			ostringstream os; \
			istringstream is; \
			property1.writeASCIIValue(os); \
			is.str(os.str()); \
			property2.readASCIIValue(is); \
			assert(property1.getValue() == property2.getValue()); \
		} \
		{ \
			ostringstream os; \
			istringstream is; \
			property1.writeBinaryValue(os); \
			is.str(os.str()); \
			property2.readBinaryValue(is); \
			assert(property1.getValue() == property2.getValue()); \
		} \
	}
	{
		cout << "Testing GProperty for glm propety types: ";

		//glm::vec2 property
		CHECK_GLM_PROPERTY(glm::vec2, GVec2Property);

		//glm::vec3 property
		CHECK_GLM_PROPERTY(glm::vec3, GVec3Property);

		//glm::vec4 property
		CHECK_GLM_PROPERTY(glm::vec4, GVec4Property);

		//glm::mat2 property
		CHECK_GLM_PROPERTY(glm::mat2, GMat2Property);

		//glm::mat3 property
		CHECK_GLM_PROPERTY(glm::mat3, GMat3Property);

		//glm::mat4 property
		CHECK_GLM_PROPERTY(glm::mat4, GMat4Property);

		cout << "Ok" << endl;
	}

	{
		cout << "Testing GProperty for string types: ";

		{ 
			GVariant gv = GVariant::create<string>(string("TestString")); \
			GStringProperty property1; \
			GStringProperty property2; \
			property1.set(gv); \
			assert(GVariant::cast<string>(property1.get()) == string("TestString")); \
			assert(property1.getValue() == string("TestString")); \
			property1.setValue(string("TestString_Modified")); \
			assert(GVariant::cast<string>(property1.get()) == string("TestString_Modified")); \
			assert(property1.getValue() == string("TestString_Modified")); \
			{ \
				ostringstream os; \
				istringstream is; \
				property1.writeASCIIValue(os); \
				is.str(os.str()); \
				property2.readASCIIValue(is); \
				assert(property1.getValue() == property2.getValue()); \
			} \
			{ \
				ostringstream os; \
				istringstream is; \
				property1.writeBinaryValue(os); \
				is.str(os.str()); \
				property2.readBinaryValue(is); \
				assert(property1.getValue() == property2.getValue()); \
			} \
		}
		cout << "Ok" << endl;
	}

	{
		cout << "Testing GProperty for Object Pointer type: ";
		shared_ptr<property_test::Node> n1 = make_shared<property_test::Node>(23);
		{
			shared_ptr<property_test::Node> n2 = make_shared<property_test::Node>(47);
			n1->interested_object.setValue(n2.get());
			assert(n1->interested_object.getValue()->id == 47);
		}
		assert(n1->interested_object.getValue() == nullptr);

		cout << "Ok" << endl;
	}

}
