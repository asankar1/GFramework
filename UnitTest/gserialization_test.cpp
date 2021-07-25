#include <fstream>

#include <gtest/gtest.h>

#include <GFramework/GReflection/GReflection.h>
#include <GFramework/GSerialization/GStringSerializer.h>
#include "gserialization_test.h""
#include <Node.h>
#include <Sphere.h>

using namespace std;
using namespace GFramework;
using namespace GFrameworkTest;

template<>
GStringSerializer& GStringSerializerWriter::write<short>(GStringSerializer& stream, short& value)
{
	*stream.getStream() << std::hex << value << " ";
	return stream;
}

template<>
GStringDeserializer& GStringDeserializerReader::read<short>(GStringDeserializer& stream, short& value)
{
	*stream.getStream() >> std::hex >> value;
	return stream;
}

class Color
{
public:
	Color() {}
	Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a) : r(_r), g(_g), b(_b), a(_a) {}
	uint8 r=0, g=0, b=0, a=0;
	bool operator==(const Color& rhs) const { return (this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a); }
};
namespace GFramework {
	class GColorProperty : public GPropertyInterface
	{
	public:
		GColorProperty(Color v = Color()) : value(v) {}

		bool operator==(const Color& rhs) const { return (value == rhs); }

		bool operator==(const GColorProperty& rhs) const { return (value == rhs.value); }

		virtual void set(GVariant& _value)
		{
			value = GVariant::cast<Color>(_value);
		}

		void setValue(Color _value)
		{
			value = _value;
		}

		virtual GVariant get() const
		{
			//return GVariant::create<Color>(value);
			return GVariant::create(value);
		}

		const Color& getValue() const
		{
			return value;
		}

		std::ostream& writeASCIIValue(std::ostream& os) const
		{
			os << value.r << " " << value.g << " " << value.b << " " << value.a << " ";
			return os;
		}

		std::istream& readASCIIValue(std::istream& is) 
		{ 
			is >> value.r >> value.g >> value.b >> value.a;
			return is; 
		}

		std::ostream& writeBinaryValue(std::ostream& os) const { return os; }

		std::istream& readBinaryValue(std::istream& is) { return is; }

	private:
		Color value;
	};
}


class Square : public GObject
{
public:
	Square() : GObject() {}
	int16 x;
	int16 y;
	virtual void initialize() {}
	virtual GMetaclass* getMetaclass() const
	{
		return GMetaNamespaceList::_global().getMetaclass("Square");
	}
	int32 getSize() const { return size.getValue(); }
	void setSize(int32 v) { size.setValue(v); }
private:
	GInt32Property size;
};

class Circle : public GObject
{
public:
	Circle() : GObject() {}
	~Circle()
	{
		int i = 0;
	}
	GInt16Property x;
	GInt16Property y;
	virtual void initialize(){}
	virtual GMetaclass* getMetaclass() const
	{
		return GMetaNamespaceList::_global().getMetaclass("Circle");
	}
	int32 getRadius() const { return radius.getValue(); }
	void setRadius(int32 v) { radius.setValue(v); }

	GObjectSharedPtr getParent() const { return parent.getValue(); }
	void setParent(GObjectSharedPtr v) { parent.setValue(v); }

	void calculateBBox() {
		boundingBox.x = x.getValue();
		boundingBox.y = y.getValue();
		boundingBox.setSize(2 * radius.getValue());
	}
	Square boundingBox;
	GColorProperty color;
	
private:
	GObjectPointerProperty parent;
	GInt32Property radius;

};

class SerializationTestClass : public GObject
{
public:
	virtual void initialize() {}
	virtual GMetaclass* getMetaclass() const
	{
		return GMetaNamespaceList::_global().getMetaclass("SerializationTestClass");
	}

	GVec2Property  get_gvec2PropertyPrivate() const { return gvec2PropertyPrivate; };
	GVec3Property  get_gvec3PropertyPrivate() const { return gvec3PropertyPrivate; };
	GVec4Property  get_gvec4PropertyPrivate() const { return gvec4PropertyPrivate; };
	GMat2Property  get_gmat2PropertyPrivate() const { return gmat2PropertyPrivate; };
	GMat3Property  get_gmat3PropertyPrivate() const { return gmat3PropertyPrivate; };
	GMat4Property  get_gmat4PropertyPrivate() const { return gmat4PropertyPrivate; };
	GBoolProperty  get_gboolPropertyPrivate() const { return gboolPropertyPrivate; };
	GCharProperty  get_gcharPropertyPrivate() const { return gcharPropertyPrivate; };
	GInt8Property  get_gint8PropertyPrivate() const { return gint8PropertyPrivate; };
	GUint8Property  get_guint8PropertyPrivate() const { return guint8PropertyPrivate; };
	GInt16Property  get_gint16PropertyPrivate() const { return gint16PropertyPrivate; };
	GUint16Property  get_guint16PropertyPrivate()const { return guint16PropertyPrivate; };
	GInt32Property  get_gint32PropertyPrivate() const { return gint32PropertyPrivate; };
	GUint32Property  get_guint32PropertyPrivate() const { return guint32PropertyPrivate; };
	GInt64Property  get_gint64PropertyPrivate() const { return gint64PropertyPrivate; };
	GUint64Property  get_guint64PropertyPrivate() const { return guint64PropertyPrivate; };
	GFloatProperty  get_gfloatPropertyPrivate() const { return gfloatPropertyPrivate; };
	GDoubleProperty  get_gdoublePropertyPrivate() const { return gdoublePropertyPrivate; };
	GStringProperty  get_gstringPropertyPrivate() const { return gstringPropertyPrivate; };
	GObjectPointerProperty  get_gobjectpointerPropertyPrivate() const { return gobjectpointerPropertyPrivate; };
	GColorProperty  get_gcolorPropertyPrivate() const { return gcolorPropertyPrivate; };

	void set_gvec2PropertyPrivate(GVec2Property  v) { gvec2PropertyPrivate = v; };
	void set_gvec3PropertyPrivate(GVec3Property  v) { gvec3PropertyPrivate = v; };
	void set_gvec4PropertyPrivate(GVec4Property  v) { gvec4PropertyPrivate = v; };
	void set_gmat2PropertyPrivate(GMat2Property  v) { gmat2PropertyPrivate = v; };
	void set_gmat3PropertyPrivate(GMat3Property  v) { gmat3PropertyPrivate = v; };
	void set_gmat4PropertyPrivate(GMat4Property  v) { gmat4PropertyPrivate = v; };
	void set_gboolPropertyPrivate(GBoolProperty  v) { gboolPropertyPrivate = v; };
	void set_gcharPropertyPrivate(GCharProperty  v) { gcharPropertyPrivate = v; };
	void set_gint8PropertyPrivate(GInt8Property  v) { gint8PropertyPrivate = v; };
	void set_guint8PropertyPrivate(GUint8Property  v) { guint8PropertyPrivate = v; };
	void set_gint16PropertyPrivate(GInt16Property  v) { gint16PropertyPrivate = v; };
	void set_guint16PropertyPrivate(GUint16Property  v) { guint16PropertyPrivate = v; };
	void set_gint32PropertyPrivate(GInt32Property  v) { gint32PropertyPrivate = v; };
	void set_guint32PropertyPrivate(GUint32Property  v) { guint32PropertyPrivate = v; };
	void set_gint64PropertyPrivate(GInt64Property  v) { gint64PropertyPrivate = v; };
	void set_guint64PropertyPrivate(GUint64Property  v) { guint64PropertyPrivate = v; };
	void set_gfloatPropertyPrivate(GFloatProperty  v) { gfloatPropertyPrivate = v; };
	void set_gdoublePropertyPrivate(GDoubleProperty  v) { gdoublePropertyPrivate = v; };
	void set_gstringPropertyPrivate(GStringProperty  v) { gstringPropertyPrivate = v; };
	void set_gobjectpointerPropertyPrivate(GObjectPointerProperty   v) { gobjectpointerPropertyPrivate = v; };
	void set_gcolorPropertyPrivate(GColorProperty   v) { gcolorPropertyPrivate = v; };
	
public:
	GVec2Property gvec2PropertyPublic;
	GVec3Property gvec3PropertyPublic;
	GVec4Property gvec4PropertyPublic;
	GMat2Property gmat2PropertyPublic;
	GMat3Property gmat3PropertyPublic;
	GMat4Property gmat4PropertyPublic;
	GBoolProperty gboolPropertyPublic;
	GCharProperty gcharPropertyPublic;
	GInt8Property gint8PropertyPublic;
	GUint8Property guint8PropertyPublic;
	GInt16Property gint16PropertyPublic;
	GUint16Property guint16PropertyPublic;
	GInt32Property gint32PropertyPublic;
	GUint32Property guint32PropertyPublic;
	GInt64Property gint64PropertyPublic;
	GUint64Property guint64PropertyPublic;
	GFloatProperty gfloatPropertyPublic;
	GDoubleProperty gdoublePropertyPublic;
	GStringProperty gstringPropertyPublic;
	GObjectPointerProperty gobjectpointerPropertyPublic;
	GColorProperty gcolorPropertyPublic;
private:
	GVec2Property gvec2PropertyPrivate;
	GVec3Property gvec3PropertyPrivate;
	GVec4Property gvec4PropertyPrivate;
	GMat2Property gmat2PropertyPrivate;
	GMat3Property gmat3PropertyPrivate;
	GMat4Property gmat4PropertyPrivate;
	GBoolProperty gboolPropertyPrivate;
	GCharProperty gcharPropertyPrivate;
	GInt8Property gint8PropertyPrivate;
	GUint8Property guint8PropertyPrivate;
	GInt16Property gint16PropertyPrivate;
	GUint16Property guint16PropertyPrivate;
	GInt32Property gint32PropertyPrivate;
	GUint32Property guint32PropertyPrivate;
	GInt64Property gint64PropertyPrivate;
	GUint64Property guint64PropertyPrivate;
	GFloatProperty gfloatPropertyPrivate;
	GDoubleProperty gdoublePropertyPrivate;
	GStringProperty gstringPropertyPrivate;
	GObjectPointerProperty gobjectpointerPropertyPrivate;
	GColorProperty gcolorPropertyPrivate;

};

BEGIN_DEFINE_META(SerializationTestClass)
GMetaNamespaceList::_global()
.define<SerializationTestClass>("SerializationTestClass")
.baseMetaclass("GObject", { "GFramework" })
.property("gvec2PropertyPrivate", &SerializationTestClass::get_gvec2PropertyPrivate, &SerializationTestClass::set_gvec2PropertyPrivate)
.property("gvec3PropertyPrivate", &SerializationTestClass::get_gvec3PropertyPrivate, &SerializationTestClass::set_gvec3PropertyPrivate)
.property("gvec4PropertyPrivate", &SerializationTestClass::get_gvec4PropertyPrivate, &SerializationTestClass::set_gvec4PropertyPrivate)
.property("gmat2PropertyPrivate", &SerializationTestClass::get_gmat2PropertyPrivate, &SerializationTestClass::set_gmat2PropertyPrivate)
.property("gmat3PropertyPrivate", &SerializationTestClass::get_gmat3PropertyPrivate, &SerializationTestClass::set_gmat3PropertyPrivate)
.property("gmat4PropertyPrivate", &SerializationTestClass::get_gmat4PropertyPrivate, &SerializationTestClass::set_gmat4PropertyPrivate)
.property("gboolPropertyPrivate", &SerializationTestClass::get_gboolPropertyPrivate, &SerializationTestClass::set_gboolPropertyPrivate)
.property("gcharPropertyPrivate", &SerializationTestClass::get_gcharPropertyPrivate, &SerializationTestClass::set_gcharPropertyPrivate)
.property("gint8PropertyPrivate", &SerializationTestClass::get_gint8PropertyPrivate, &SerializationTestClass::set_gint8PropertyPrivate)
.property("guint8PropertyPrivate", &SerializationTestClass::get_guint8PropertyPrivate, &SerializationTestClass::set_guint8PropertyPrivate)
.property("gint16PropertyPrivate", &SerializationTestClass::get_gint16PropertyPrivate, &SerializationTestClass::set_gint16PropertyPrivate)
.property("guint16PropertyPrivate", &SerializationTestClass::get_guint16PropertyPrivate, &SerializationTestClass::set_guint16PropertyPrivate)
.property("gint32PropertyPrivate", &SerializationTestClass::get_gint32PropertyPrivate, &SerializationTestClass::set_gint32PropertyPrivate)
.property("guint32PropertyPrivate", &SerializationTestClass::get_guint32PropertyPrivate, &SerializationTestClass::set_guint32PropertyPrivate)
.property("gint64PropertyPrivate", &SerializationTestClass::get_gint64PropertyPrivate, &SerializationTestClass::set_gint64PropertyPrivate)
.property("guint64PropertyPrivate", &SerializationTestClass::get_guint64PropertyPrivate, &SerializationTestClass::set_guint64PropertyPrivate)
.property("gfloatPropertyPrivate", &SerializationTestClass::get_gfloatPropertyPrivate, &SerializationTestClass::set_gfloatPropertyPrivate)
.property("gdoublePropertyPrivate", &SerializationTestClass::get_gdoublePropertyPrivate, &SerializationTestClass::set_gdoublePropertyPrivate)
.property("gstringPropertyPrivate", &SerializationTestClass::get_gstringPropertyPrivate, &SerializationTestClass::set_gstringPropertyPrivate)
.property("gobjectpointerPropertyPrivate", &SerializationTestClass::get_gobjectpointerPropertyPrivate, &SerializationTestClass::set_gobjectpointerPropertyPrivate)
.property("gcolorPropertyPrivate", &SerializationTestClass::get_gcolorPropertyPrivate, &SerializationTestClass::set_gcolorPropertyPrivate)
.property("gvec2PropertyPublic", &SerializationTestClass::gvec2PropertyPublic)
.property("gvec3PropertyPublic", &SerializationTestClass::gvec3PropertyPublic)
.property("gvec4PropertyPublic", &SerializationTestClass::gvec4PropertyPublic)
.property("gmat2PropertyPublic", &SerializationTestClass::gmat2PropertyPublic)
.property("gmat3PropertyPublic", &SerializationTestClass::gmat3PropertyPublic)
.property("gmat4PropertyPublic", &SerializationTestClass::gmat4PropertyPublic)
.property("gboolPropertyPublic", &SerializationTestClass::gboolPropertyPublic)
.property("gcharPropertyPublic", &SerializationTestClass::gcharPropertyPublic)
.property("gint8PropertyPublic", &SerializationTestClass::gint8PropertyPublic)
.property("guint8PropertyPublic", &SerializationTestClass::guint8PropertyPublic)
.property("gint16PropertyPublic", &SerializationTestClass::gint16PropertyPublic)
.property("guint16PropertyPublic", &SerializationTestClass::guint16PropertyPublic)
.property("gint32PropertyPublic", &SerializationTestClass::gint32PropertyPublic)
.property("guint32PropertyPublic", &SerializationTestClass::guint32PropertyPublic)
.property("gint64PropertyPublic", &SerializationTestClass::gint64PropertyPublic)
.property("guint64PropertyPublic", &SerializationTestClass::guint64PropertyPublic)
.property("gfloatPropertyPublic", &SerializationTestClass::gfloatPropertyPublic)
.property("gdoublePropertyPublic", &SerializationTestClass::gdoublePropertyPublic)
.property("gstringPropertyPublic", &SerializationTestClass::gstringPropertyPublic)
.property("gobjectpointerPropertyPublic", &SerializationTestClass::gobjectpointerPropertyPublic)
.property("gcolorPropertyPublic", &SerializationTestClass::gcolorPropertyPublic)
;
END_DEFINE_META(SerializationTestClass)

BEGIN_DEFINE_META(Circle)
	GMetaNamespaceList::_global()
	.define<Circle>("Circle")
	.property("radius", &Circle::getRadius, &Circle::setRadius)
	.property("x", &Circle::x)
	.property("y", &Circle::y)
	.property("Color", &Circle::color)
	.property("parent", &Circle::getParent, &Circle::setParent)
	/*.property("boundingbox", &Circle::boundingBox)*/;
END_DEFINE_META(Circle)

/*BEGIN_DEFINE_META(Square)
	GMetaNamespaceList::_global()
	.define<Square>("Square")
	.property("size", &Square::getSize, &Square::setSize)
	.property("x", &Square::x)
	.property("y", &Square::y);
END_DEFINE_META(Square)*/

#define SETUP_PUBLIC_PROPERTY(PROP, VALUE) \
		auto PROP##_assigner = [&]() {write_obj->PROP## = VALUE; }; \
		PROP##_assigner(); \
		auto PROP##_verifier = [&](SerializationTestClass* read_obj)->bool {return (((SerializationTestClass*)read_obj)->PROP## == VALUE); }; 

#define SETUP_PRIVATE_PROPERTY(PROP, VALUE) \
		auto PROP##_assigner = [&]() {write_obj->set_##PROP##(VALUE); }; \
		PROP##_assigner(); \
		auto PROP##_verifier = [&](SerializationTestClass* read_obj)->bool {return (((SerializationTestClass*)read_obj)->get_##PROP##() == VALUE); }; 

#define VERIFY_PROPERTY(PROP, VALUE) \
		PROP##_verifier(VALUE); 

class GSerialization$$GPropertyTypes : public ::testing::Test {
protected:
	void SetUp() override {
		//setup public properties
		SETUP_PUBLIC_PROPERTY(gvec2PropertyPublic, vec2_val);
		SETUP_PUBLIC_PROPERTY(gvec3PropertyPublic, vec3_val);
		SETUP_PUBLIC_PROPERTY(gvec4PropertyPublic, vec4_val);
		SETUP_PUBLIC_PROPERTY(gmat2PropertyPublic, mat2_val);
		SETUP_PUBLIC_PROPERTY(gmat3PropertyPublic, mat3_val);
		SETUP_PUBLIC_PROPERTY(gmat4PropertyPublic, mat4_val);
		SETUP_PUBLIC_PROPERTY(gboolPropertyPublic, bool_val);
		SETUP_PUBLIC_PROPERTY(gcharPropertyPublic, char_val);
		SETUP_PUBLIC_PROPERTY(gint8PropertyPublic, int8_val);
		SETUP_PUBLIC_PROPERTY(guint8PropertyPublic, uint8_val);
		SETUP_PUBLIC_PROPERTY(gint16PropertyPublic, int16_val);
		SETUP_PUBLIC_PROPERTY(guint16PropertyPublic, uint16_val);
		SETUP_PUBLIC_PROPERTY(gint32PropertyPublic, int32_val);
		SETUP_PUBLIC_PROPERTY(guint32PropertyPublic, uint32_val);
		SETUP_PUBLIC_PROPERTY(gint64PropertyPublic, int64_val);
		SETUP_PUBLIC_PROPERTY(guint64PropertyPublic, uint64_val);
		SETUP_PUBLIC_PROPERTY(gfloatPropertyPublic, float_val);
		SETUP_PUBLIC_PROPERTY(gstringPropertyPublic, string_val);
		SETUP_PUBLIC_PROPERTY(gdoublePropertyPublic, double_val);
		SETUP_PUBLIC_PROPERTY(gobjectpointerPropertyPublic, gobject_ptr);
		SETUP_PUBLIC_PROPERTY(gcolorPropertyPublic, blue);

		//setup private properties
		SETUP_PRIVATE_PROPERTY(gvec2PropertyPrivate, vec2_val);
		SETUP_PRIVATE_PROPERTY(gvec3PropertyPrivate, vec3_val);
		SETUP_PRIVATE_PROPERTY(gvec4PropertyPrivate, vec4_val);
		SETUP_PRIVATE_PROPERTY(gmat2PropertyPrivate, mat2_val);
		SETUP_PRIVATE_PROPERTY(gmat3PropertyPrivate, mat3_val);
		SETUP_PRIVATE_PROPERTY(gmat4PropertyPrivate, mat4_val);
		SETUP_PRIVATE_PROPERTY(gboolPropertyPrivate, bool_val);
		SETUP_PRIVATE_PROPERTY(gcharPropertyPrivate, char_val);
		SETUP_PRIVATE_PROPERTY(gint8PropertyPrivate, int8_val);
		SETUP_PRIVATE_PROPERTY(guint8PropertyPrivate, uint8_val);
		SETUP_PRIVATE_PROPERTY(gint16PropertyPrivate, int16_val);
		SETUP_PRIVATE_PROPERTY(guint16PropertyPrivate, uint16_val);
		SETUP_PRIVATE_PROPERTY(gint32PropertyPrivate, int32_val);
		SETUP_PRIVATE_PROPERTY(guint32PropertyPrivate, uint32_val);
		SETUP_PRIVATE_PROPERTY(gint64PropertyPrivate, int64_val);
		SETUP_PRIVATE_PROPERTY(guint64PropertyPrivate, uint64_val);
		SETUP_PRIVATE_PROPERTY(gfloatPropertyPrivate, float_val);
		SETUP_PRIVATE_PROPERTY(gdoublePropertyPrivate, double_val);
		SETUP_PRIVATE_PROPERTY(gstringPropertyPrivate, string_val);
		SETUP_PRIVATE_PROPERTY(gobjectpointerPropertyPrivate, gobject_ptr);
		SETUP_PRIVATE_PROPERTY(gcolorPropertyPrivate, blue);
		
		//setup String out serializer and stream
		out_text = make_shared<ofstream>();
		out_text->open(filename, ios_base::out);
		if (!out_text->is_open())
		{
			cout << endl << "Unable to open file:" << filename << endl << endl;
			return;
		}
		stringSer.open(out_text);
		stringSer << write_obj;
		stringSer.close();


		//setup String out deserializer and stream
		in_text = make_shared<ifstream>();
		in_text->open(filename, ios_base::in);
		if (!in_text->is_open())
		{
			cout << endl << "Unable to open file:" << filename << endl << endl;
			return;
		}
		stringDeSer.open(in_text);
		stringDeSer >> &read_obj;
		ptr = (SerializationTestClass*)read_obj.get();
		stringDeSer.close();
	}

	void TearDown() override {
		out_text->close();
		in_text->close();
	}

protected:
	shared_ptr<SerializationTestClass> write_obj = make_shared<SerializationTestClass>();
	GObjectSharedPtr read_obj = nullptr;
	SerializationTestClass* ptr = nullptr;

	glm::vec2 vec2_val = glm::vec2(1, 2);
	glm::vec3 vec3_val = glm::vec3(3, 4, 5);
	glm::vec4 vec4_val = glm::vec4(6, 7, 8, 9);
	glm::mat2 mat2_val = glm::mat2();
	glm::mat3 mat3_val = glm::mat3();
	glm::mat4 mat4_val = glm::mat4();
	bool bool_val = true;
	char char_val = 'A';
	int8 int8_val = 123;
	uint8 uint8_val = 456;
	int16 int16_val = 123;
	uint16 uint16_val = 456;
	int32 int32_val = 123;
	uint32 uint32_val = 456;
	int64 int64_val = 123;
	uint64 uint64_val = 456;
	float float_val = 3.14f;
	double double_val = 3.14;
	string string_val = "default string value";
	GObjectSharedPtr gobject_ptr = GObjectSharedPtr(nullptr);
	Color blue = { 0, 0, 255, 255 };

	std::shared_ptr<ofstream> out_text;
	std::shared_ptr<ifstream> in_text;
	string filename = "ser_test.txt";
	GStringSerializer stringSer;
	GStringDeserializer stringDeSer;
};

#define GTEST_GREFLECTION_PUBLIC(type) \
	TEST_F(GSerialization$$GPropertyTypes, type) { \
		EXPECT_EQ(std::static_pointer_cast<SerializationTestClass>(read_obj)->type, write_obj->type); \
	}

#define GTEST_GREFLECTION_PRIVATE(type) \
	TEST_F(GSerialization$$GPropertyTypes, type) { \
		EXPECT_EQ(std::static_pointer_cast<SerializationTestClass>(read_obj)->get_##type##(), write_obj->get_##type##()); \
	}

GTEST_GREFLECTION_PUBLIC(gvec2PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gvec3PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gvec4PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gmat2PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gmat3PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gmat4PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gboolPropertyPublic)
GTEST_GREFLECTION_PUBLIC(gcharPropertyPublic)
GTEST_GREFLECTION_PUBLIC(gint8PropertyPublic)
GTEST_GREFLECTION_PUBLIC(guint8PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gint16PropertyPublic)
GTEST_GREFLECTION_PUBLIC(guint16PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gint32PropertyPublic)
GTEST_GREFLECTION_PUBLIC(guint32PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gint64PropertyPublic)
GTEST_GREFLECTION_PUBLIC(guint64PropertyPublic)
GTEST_GREFLECTION_PUBLIC(gfloatPropertyPublic)
GTEST_GREFLECTION_PUBLIC(gstringPropertyPublic)
GTEST_GREFLECTION_PUBLIC(gdoublePropertyPublic)
GTEST_GREFLECTION_PUBLIC(gobjectpointerPropertyPublic)
GTEST_GREFLECTION_PUBLIC(gcolorPropertyPublic)

GTEST_GREFLECTION_PRIVATE(gvec2PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gvec3PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gvec4PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gmat2PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gmat3PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gmat4PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gboolPropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gcharPropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gint8PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(guint8PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gint16PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(guint16PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gint32PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(guint32PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gint64PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(guint64PropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gfloatPropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gstringPropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gdoublePropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gobjectpointerPropertyPrivate)
GTEST_GREFLECTION_PRIVATE(gcolorPropertyPrivate)

#if 0
void run_serialization_testcases()
{
	cout << "================================" << endl;
	cout << "Startig GSerialization testcases" << endl;
	cout << "================================" << endl;
	
/*	NodeSharedPtr nullparent(NULL);
	NodeSharedPtr osphere1_ptr = make_shared<sphere>("sphere1", nullparent, 25);
	sphere osphere2("sphere2", osphere1_ptr, 15);
*/
#if 1
	cout << "\nPerform serialization and deserialization with text file format:" << endl;
	{
		//Intialize test case contents
		shared_ptr<SerializationTestClass> write_obj = make_shared<SerializationTestClass>();

		auto vec2_val = glm::vec2(1, 2);
		auto vec3_val = glm::vec3(3, 4, 5);
		auto vec4_val = glm::vec4(6, 7, 8, 9);
		auto mat2_val = glm::mat2();
		auto mat3_val = glm::mat3();
		auto mat4_val = glm::mat4();
		auto bool_val = true;
		char char_val = 'A';
		int8 int8_val = 123;
		uint8 uint8_val = 456;
		int16 int16_val = 123;
		uint16 uint16_val = 456;
		int32 int32_val = 123;
		uint32 uint32_val = 456;
		int64 int64_val = 123;
		uint64 uint64_val = 456;
		float float_val = 3.14f;
		double double_val = 3.14;
		string string_val = "default string value";
		GObjectSharedPtr gobject_ptr = GObjectSharedPtr(nullptr);
		Color blue(0, 0, 255, 255);


		
		//auto assigner = [&]() {write_obj->gvec2PropertyPublic = vec2_val; };
		//auto verifier = [&](SerializationTestClass* read_obj) {assert(((SerializationTestClass*)read_obj)->gvec2PropertyPublic == vec2_val); };

		SETUP_PUBLIC_PROPERTY(gvec2PropertyPublic, vec2_val);
		SETUP_PUBLIC_PROPERTY(gvec3PropertyPublic, vec3_val);
		SETUP_PUBLIC_PROPERTY(gvec4PropertyPublic, vec4_val);
		SETUP_PUBLIC_PROPERTY(gmat2PropertyPublic, mat2_val);
		SETUP_PUBLIC_PROPERTY(gmat3PropertyPublic, mat3_val);
		SETUP_PUBLIC_PROPERTY(gmat4PropertyPublic, mat4_val);
		SETUP_PUBLIC_PROPERTY(gboolPropertyPublic, bool_val);
		SETUP_PUBLIC_PROPERTY(gcharPropertyPublic, char_val);
		SETUP_PUBLIC_PROPERTY(gint8PropertyPublic, int8_val);
		SETUP_PUBLIC_PROPERTY(guint8PropertyPublic, uint8_val);
		SETUP_PUBLIC_PROPERTY(gint16PropertyPublic, int16_val);
		SETUP_PUBLIC_PROPERTY(guint16PropertyPublic, uint16_val);
		SETUP_PUBLIC_PROPERTY(gint32PropertyPublic, int32_val);
		SETUP_PUBLIC_PROPERTY(guint32PropertyPublic, uint32_val);
		SETUP_PUBLIC_PROPERTY(gint64PropertyPublic, int64_val);
		SETUP_PUBLIC_PROPERTY(guint64PropertyPublic, uint64_val);
		SETUP_PUBLIC_PROPERTY(gfloatPropertyPublic, float_val);
		SETUP_PUBLIC_PROPERTY(gstringPropertyPublic, string_val);
		SETUP_PUBLIC_PROPERTY(gdoublePropertyPublic, double_val);
		SETUP_PUBLIC_PROPERTY(gobjectpointerPropertyPublic, gobject_ptr);
		SETUP_PUBLIC_PROPERTY(gcolorPropertyPublic, blue);
		
		/*auto gobjectpointerPropertyPublic_assigner = [&]() {write_obj->gobjectpointerPropertyPublic = gobject_ptr; };
		gobjectpointerPropertyPublic_assigner();
		auto gobjectpointerPropertyPublic_verifier = [&](SerializationTestClass* read_obj) {
			assert(((SerializationTestClass*)read_obj)->gobjectpointerPropertyPublic == gobject_ptr); 
		};*/

		//TODO: find a way to use assignment operator for the GObjectPointer
		//write_obj->gobjectpointerPropertyPublic.setValue(gobject_ptr);

		SETUP_PRIVATE_PROPERTY(gvec2PropertyPrivate, vec2_val);
		SETUP_PRIVATE_PROPERTY(gvec3PropertyPrivate, vec3_val);
		SETUP_PRIVATE_PROPERTY(gvec4PropertyPrivate, vec4_val);
		SETUP_PRIVATE_PROPERTY(gmat2PropertyPrivate, mat2_val);
		SETUP_PRIVATE_PROPERTY(gmat3PropertyPrivate, mat3_val);
		SETUP_PRIVATE_PROPERTY(gmat4PropertyPrivate, mat4_val);
		SETUP_PRIVATE_PROPERTY(gboolPropertyPrivate, bool_val);
		SETUP_PRIVATE_PROPERTY(gcharPropertyPrivate, char_val);
		SETUP_PRIVATE_PROPERTY(gint8PropertyPrivate, int8_val);
		SETUP_PRIVATE_PROPERTY(guint8PropertyPrivate, uint8_val);
		SETUP_PRIVATE_PROPERTY(gint16PropertyPrivate, int16_val);
		SETUP_PRIVATE_PROPERTY(guint16PropertyPrivate, uint16_val);
		SETUP_PRIVATE_PROPERTY(gint32PropertyPrivate, int32_val);
		SETUP_PRIVATE_PROPERTY(guint32PropertyPrivate, uint32_val);
		SETUP_PRIVATE_PROPERTY(gint64PropertyPrivate, int64_val);
		SETUP_PRIVATE_PROPERTY(guint64PropertyPrivate, uint64_val);
		SETUP_PRIVATE_PROPERTY(gfloatPropertyPrivate, float_val);
		SETUP_PRIVATE_PROPERTY(gdoublePropertyPrivate, double_val);
		SETUP_PRIVATE_PROPERTY(gstringPropertyPrivate, string_val);
		SETUP_PRIVATE_PROPERTY(gobjectpointerPropertyPrivate, gobject_ptr);
		SETUP_PRIVATE_PROPERTY(gcolorPropertyPrivate, blue);
		//TODO: find a way to test the below
		//write_obj->set_gobjectpointerPropertyPrivate(gobject_ptr);

		//Text serialization
		auto out_text = make_shared<ofstream>();
		string filename = "ser_test.txt";
		out_text->open(filename, ios_base::out);
		if (!out_text->is_open())
		{
			cout << endl << "Unable to open file:" << filename << endl << endl;
			return;
		}
		GStringSerializer ts;
		ts.open(out_text);
		ts << write_obj;
		ts.close();
		out_text->close();

		//Text deserialization
		auto in_text = make_shared<ifstream>();
		in_text->open(filename, ios_base::in);
		if (!in_text->is_open())
		{
			cout << endl << "Unable to open file:" << filename << endl << endl;
			return;
		}
		GStringDeserializer tds;
		tds.open(in_text);
		GObjectSharedPtr read_obj = nullptr;
		tds >> &read_obj;
		tds.close();
		in_text->close();
		auto ptr = (SerializationTestClass*)read_obj.get();
		//assert(((SerializationTestClass*)read_obj.get())->gvec2PropertyPublic == vec2_val);
		VERIFY_PROPERTY(gvec2PropertyPublic, ptr);
		VERIFY_PROPERTY(gvec3PropertyPublic, ptr);
		VERIFY_PROPERTY(gvec4PropertyPublic, ptr);
		VERIFY_PROPERTY(gmat2PropertyPublic, ptr);
		VERIFY_PROPERTY(gmat3PropertyPublic, ptr);
		VERIFY_PROPERTY(gmat4PropertyPublic, ptr);
		VERIFY_PROPERTY(gboolPropertyPublic, ptr);
		VERIFY_PROPERTY(gcharPropertyPublic, ptr);
		VERIFY_PROPERTY(gint8PropertyPublic, ptr);
		VERIFY_PROPERTY(guint8PropertyPublic, ptr);
		VERIFY_PROPERTY(gint16PropertyPublic, ptr);
		VERIFY_PROPERTY(guint16PropertyPublic, ptr);
		VERIFY_PROPERTY(gint32PropertyPublic, ptr);
		VERIFY_PROPERTY(guint32PropertyPublic, ptr);
		VERIFY_PROPERTY(gint64PropertyPublic, ptr);
		VERIFY_PROPERTY(guint64PropertyPublic, ptr);
		VERIFY_PROPERTY(gfloatPropertyPublic, ptr);
		VERIFY_PROPERTY(gdoublePropertyPublic, ptr);
		VERIFY_PROPERTY(gstringPropertyPublic, ptr);
		//TODO: find a way to use assignment operator for the GObjectPointer
		//assert(ptr->gobjectpointerPropertyPublic.getValue() == gobject_ptr);
		VERIFY_PROPERTY(gobjectpointerPropertyPublic, ptr);
		VERIFY_PROPERTY(gcolorPropertyPublic, ptr);

		VERIFY_PROPERTY(gvec2PropertyPrivate, ptr);
		VERIFY_PROPERTY(gvec3PropertyPrivate, ptr);
		VERIFY_PROPERTY(gvec4PropertyPrivate, ptr);
		VERIFY_PROPERTY(gmat2PropertyPrivate, ptr);
		VERIFY_PROPERTY(gmat3PropertyPrivate, ptr);
		VERIFY_PROPERTY(gmat4PropertyPrivate, ptr);
		VERIFY_PROPERTY(gboolPropertyPrivate, ptr);
		VERIFY_PROPERTY(gcharPropertyPrivate, ptr);
		VERIFY_PROPERTY(gint8PropertyPrivate, ptr);
		VERIFY_PROPERTY(guint8PropertyPrivate, ptr);
		VERIFY_PROPERTY(gint16PropertyPrivate, ptr);
		VERIFY_PROPERTY(guint16PropertyPrivate, ptr);
		VERIFY_PROPERTY(gint32PropertyPrivate, ptr);
		VERIFY_PROPERTY(guint32PropertyPrivate, ptr);
		VERIFY_PROPERTY(gint64PropertyPrivate, ptr);
		VERIFY_PROPERTY(guint64PropertyPrivate, ptr);
		VERIFY_PROPERTY(gfloatPropertyPrivate, ptr);
		VERIFY_PROPERTY(gdoublePropertyPrivate, ptr);
		VERIFY_PROPERTY(gstringPropertyPrivate, ptr);
		VERIFY_PROPERTY(gobjectpointerPropertyPrivate, ptr);
		VERIFY_PROPERTY(gcolorPropertyPrivate, ptr);
	}
#endif
#if 0
	{
		//Text serialization
		auto out_text = make_shared<ofstream>();
		out_text->open("ser_test.txt", ios_base::out);
		if (!out_text->is_open())
		{
			return;
		}

		Circle ic1;
		ic1.setRadius(45);
		ic1.x = 100;
		ic1.y = 50;
		ic1.color.setValue(Color(4, 5, 6, 7));

		auto parent_circle = make_shared<Circle>();
		parent_circle->rename("parent_circle");
		parent_circle->setRadius(90);
		parent_circle->x = 1000;
		parent_circle->y = 500;
		parent_circle->color.setValue(Color(44, 55, 66, 77));

		auto grand_parent_circle = make_shared<Circle>();
		grand_parent_circle->rename("grnd parent_circle");
		grand_parent_circle->setRadius(900);
		grand_parent_circle ->x = 10000;
		grand_parent_circle->y = 5000;
		grand_parent_circle->color.setValue(Color(444, 555, 666, 777));

		parent_circle->setParent(grand_parent_circle);
		ic1.setParent( parent_circle);

		Square is1;
		is1.setSize(17);
		is1.x = 93;
		is1.y = 74;

		GStringSerializer ts;
		ts.open(out_text);
		GInt32Property i = 4;
		GInt16Property s = 2;

		GColorProperty c(Color(1, 2, 3, 4));
		 
		
		ts << s << i << s << c;
		

		ts << ic1;
		//ts << is1;
		
		ts.close();
		out_text->close();

		//Text deserialization
		auto in_text = make_shared<ifstream>();
		in_text->open("ser_test.txt", ios_base::in);
		if (!in_text->is_open())
		{
			return;
		}
		GStringDeserializer tds;
		tds.open(in_text);
		GInt32Property ii = 4;
		GInt16Property ss = 2;
		GColorProperty cc(Color(1, 2, 3, 4));
		GObjectSharedPtr op1 = nullptr;
		GObject* op2 = nullptr;
		//tds >> ss >> ii >> ss >> cc;
		tds >> &op1;

		/*

		assert(((Circle*)op1)->getRadius() == ic1.getRadius());
		assert(((Circle*)op1)->x == ic1.x);
		assert(((Circle*)op1)->y == ic1.y);

		assert(((Square*)op2)->getSize() == is1.getSize());
		assert(((Square*)op2)->x == is1.x);
		assert(((Square*)op2)->y == is1.y);*/

		/*tds >> oc1r;
		tds >> oc2;
		tds >> oc3;
		tds >> ocp;*/
		//tds >> output;
		

		tds.close();
		in_text->close();
		tds.resolveDependencies();
		auto getCirclePtr = [&](GObjectSharedPtr obj)->Circle* { return ((Circle*)obj.get()); };
		assert(getCirclePtr(op1)->x == 100);
		assert(getCirclePtr(getCirclePtr(op1)->getParent())->x == 1000);
		assert(getCirclePtr(getCirclePtr(getCirclePtr(op1)->getParent())->getParent())->x == 10000);

		//assert(os1->getName() == static_cast<sphere*>(is1)->getName());
		//assert(os1->getRadius() == static_cast<sphere*>(is1)->getRadius());
		
	}
#endif
#if 0
	{
		//Binary serialization
		GBinarySerializer bs;
		bs.open("test.bin");
		bs << os1 << os2;
		bs.close();

		//Binary serialization
		GBinaryDeSerializer bds;
		bds.open("test.bin");
		GObject* is1 = nullptr;
		GObject* is2 = nullptr;
		(bds >> &is1) >> &is2;
		bds.close();
		bds.resolveDependencies();
	}
#endif
	//osphere1_ptr.reset();
}
#endif