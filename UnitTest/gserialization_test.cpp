#include <fstream>
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
	bool operator==(const Color& rhs) { return (this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a); }
};
namespace GFramework {
	class GColorProperty : public GPropertyInterface
	{
	public:
		GColorProperty(Color v = Color()) : value(v) {}

		bool operator==(const Color& rhs) { return (value == rhs); }

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
			return GVariant::create<Color>(value);
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

	GVec2Property  get_gvec2property_private() const { return gvec2property_private; };
	GVec3Property  get_gvec3property_private() const { return gvec3property_private; };
	GVec4Property  get_gvec4property_private() const { return gvec4property_private; };
	GMat2Property  get_gmat2property_private() const { return gmat2property_private; };
	GMat3Property  get_gmat3property_private() const { return gmat3property_private; };
	GMat4Property  get_gmat4property_private() const { return gmat4property_private; };
	GBoolProperty  get_gboolproperty_private() const { return gboolproperty_private; };
	GCharProperty  get_gcharproperty_private() const { return gcharproperty_private; };
	GInt8Property  get_gint8property_private() const { return gint8property_private; };
	GUint8Property  get_guint8property_private() const { return guint8property_private; };
	GInt16Property  get_gint16property_private() const { return gint16property_private; };
	GUint16Property  get_guint16property_private()const { return guint16property_private; };
	GInt32Property  get_gint32property_private() const { return gint32property_private; };
	GUint32Property  get_guint32property_private() const { return guint32property_private; };
	GInt64Property  get_gint64property_private() const { return gint64property_private; };
	GUint64Property  get_guint64property_private() const { return guint64property_private; };
	GFloatProperty  get_gfloatproperty_private() const { return gfloatproperty_private; };
	GDoubleProperty  get_gdoubleproperty_private() const { return gdoubleproperty_private; };
	GStringProperty  get_gstringproperty_private() const { return gstringproperty_private; };
	GObjectPointerProperty  get_gobjectpointerproperty_private() const { return gobjectpointerproperty_private; };
	GColorProperty  get_gcolorproperty_private() const { return gcolorproperty_private; };

	void set_gvec2property_private(GVec2Property  v) { gvec2property_private = v; };
	void set_gvec3property_private(GVec3Property  v) { gvec3property_private = v; };
	void set_gvec4property_private(GVec4Property  v) { gvec4property_private = v; };
	void set_gmat2property_private(GMat2Property  v) { gmat2property_private = v; };
	void set_gmat3property_private(GMat3Property  v) { gmat3property_private = v; };
	void set_gmat4property_private(GMat4Property  v) { gmat4property_private = v; };
	void set_gboolproperty_private(GBoolProperty  v) { gboolproperty_private = v; };
	void set_gcharproperty_private(GCharProperty  v) { gcharproperty_private = v; };
	void set_gint8property_private(GInt8Property  v) { gint8property_private = v; };
	void set_guint8property_private(GUint8Property  v) { guint8property_private = v; };
	void set_gint16property_private(GInt16Property  v) { gint16property_private = v; };
	void set_guint16property_private(GUint16Property  v) { guint16property_private = v; };
	void set_gint32property_private(GInt32Property  v) { gint32property_private = v; };
	void set_guint32property_private(GUint32Property  v) { guint32property_private = v; };
	void set_gint64property_private(GInt64Property  v) { gint64property_private = v; };
	void set_guint64property_private(GUint64Property  v) { guint64property_private = v; };
	void set_gfloatproperty_private(GFloatProperty  v) { gfloatproperty_private = v; };
	void set_gdoubleproperty_private(GDoubleProperty  v) { gdoubleproperty_private = v; };
	void set_gstringproperty_private(GStringProperty  v) { gstringproperty_private = v; };
	void set_gobjectpointerproperty_private(GObjectPointerProperty   v) { gobjectpointerproperty_private = v; };
	void set_gcolorproperty_private(GColorProperty   v) { gcolorproperty_private = v; };
	
public:
	GVec2Property gvec2property_public;
	GVec3Property gvec3property_public;
	GVec4Property gvec4property_public;
	GMat2Property gmat2property_public;
	GMat3Property gmat3property_public;
	GMat4Property gmat4property_public;
	GBoolProperty gboolproperty_public;
	GCharProperty gcharproperty_public;
	GInt8Property gint8property_public;
	GUint8Property guint8property_public;
	GInt16Property gint16property_public;
	GUint16Property guint16property_public;
	GInt32Property gint32property_public;
	GUint32Property guint32property_public;
	GInt64Property gint64property_public;
	GUint64Property guint64property_public;
	GFloatProperty gfloatproperty_public;
	GDoubleProperty gdoubleproperty_public;
	GStringProperty gstringproperty_public;
	GObjectPointerProperty gobjectpointerproperty_public;
	GColorProperty gcolorproperty_public;
private:
	GVec2Property gvec2property_private;
	GVec3Property gvec3property_private;
	GVec4Property gvec4property_private;
	GMat2Property gmat2property_private;
	GMat3Property gmat3property_private;
	GMat4Property gmat4property_private;
	GBoolProperty gboolproperty_private;
	GCharProperty gcharproperty_private;
	GInt8Property gint8property_private;
	GUint8Property guint8property_private;
	GInt16Property gint16property_private;
	GUint16Property guint16property_private;
	GInt32Property gint32property_private;
	GUint32Property guint32property_private;
	GInt64Property gint64property_private;
	GUint64Property guint64property_private;
	GFloatProperty gfloatproperty_private;
	GDoubleProperty gdoubleproperty_private;
	GStringProperty gstringproperty_private;
	GObjectPointerProperty gobjectpointerproperty_private;
	GColorProperty gcolorproperty_private;

};

BEGIN_DEFINE_META(SerializationTestClass)
GMetaNamespaceList::_global()
.define<SerializationTestClass>("SerializationTestClass")
.baseMetaclass("GObject", { "GFramework" })
.property("gvec2property_private", &SerializationTestClass::get_gvec2property_private, &SerializationTestClass::set_gvec2property_private)
.property("gvec3property_private", &SerializationTestClass::get_gvec3property_private, &SerializationTestClass::set_gvec3property_private)
.property("gvec4property_private", &SerializationTestClass::get_gvec4property_private, &SerializationTestClass::set_gvec4property_private)
.property("gmat2property_private", &SerializationTestClass::get_gmat2property_private, &SerializationTestClass::set_gmat2property_private)
.property("gmat3property_private", &SerializationTestClass::get_gmat3property_private, &SerializationTestClass::set_gmat3property_private)
.property("gmat4property_private", &SerializationTestClass::get_gmat4property_private, &SerializationTestClass::set_gmat4property_private)
.property("gboolproperty_private", &SerializationTestClass::get_gboolproperty_private, &SerializationTestClass::set_gboolproperty_private)
.property("gcharproperty_private", &SerializationTestClass::get_gcharproperty_private, &SerializationTestClass::set_gcharproperty_private)
.property("gint8property_private", &SerializationTestClass::get_gint8property_private, &SerializationTestClass::set_gint8property_private)
.property("guint8property_private", &SerializationTestClass::get_guint8property_private, &SerializationTestClass::set_guint8property_private)
.property("gint16property_private", &SerializationTestClass::get_gint16property_private, &SerializationTestClass::set_gint16property_private)
.property("guint16property_private", &SerializationTestClass::get_guint16property_private, &SerializationTestClass::set_guint16property_private)
.property("gint32property_private", &SerializationTestClass::get_gint32property_private, &SerializationTestClass::set_gint32property_private)
.property("guint32property_private", &SerializationTestClass::get_guint32property_private, &SerializationTestClass::set_guint32property_private)
.property("gint64property_private", &SerializationTestClass::get_gint64property_private, &SerializationTestClass::set_gint64property_private)
.property("guint64property_private", &SerializationTestClass::get_guint64property_private, &SerializationTestClass::set_guint64property_private)
.property("gfloatproperty_private", &SerializationTestClass::get_gfloatproperty_private, &SerializationTestClass::set_gfloatproperty_private)
.property("gdoubleproperty_private", &SerializationTestClass::get_gdoubleproperty_private, &SerializationTestClass::set_gdoubleproperty_private)
.property("gstringproperty_private", &SerializationTestClass::get_gstringproperty_private, &SerializationTestClass::set_gstringproperty_private)
.property("gobjectpointerproperty_private", &SerializationTestClass::get_gobjectpointerproperty_private, &SerializationTestClass::set_gobjectpointerproperty_private)
.property("gcolorproperty_private", &SerializationTestClass::get_gcolorproperty_private, &SerializationTestClass::set_gcolorproperty_private)
.property("gvec2property_public", &SerializationTestClass::gvec2property_public)
.property("gvec3property_public", &SerializationTestClass::gvec3property_public)
.property("gvec4property_public", &SerializationTestClass::gvec4property_public)
.property("gmat2property_public", &SerializationTestClass::gmat2property_public)
.property("gmat3property_public", &SerializationTestClass::gmat3property_public)
.property("gmat4property_public", &SerializationTestClass::gmat4property_public)
.property("gboolproperty_public", &SerializationTestClass::gboolproperty_public)
.property("gcharproperty_public", &SerializationTestClass::gcharproperty_public)
.property("gint8property_public", &SerializationTestClass::gint8property_public)
.property("guint8property_public", &SerializationTestClass::guint8property_public)
.property("gint16property_public", &SerializationTestClass::gint16property_public)
.property("guint16property_public", &SerializationTestClass::guint16property_public)
.property("gint32property_public", &SerializationTestClass::gint32property_public)
.property("guint32property_public", &SerializationTestClass::guint32property_public)
.property("gint64property_public", &SerializationTestClass::gint64property_public)
.property("guint64property_public", &SerializationTestClass::guint64property_public)
.property("gfloatproperty_public", &SerializationTestClass::gfloatproperty_public)
.property("gdoubleproperty_public", &SerializationTestClass::gdoubleproperty_public)
.property("gstringproperty_public", &SerializationTestClass::gstringproperty_public)
.property("gobjectpointerproperty_public", &SerializationTestClass::gobjectpointerproperty_public)
.property("gcolorproperty_public", &SerializationTestClass::gcolorproperty_public)
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
		auto PROP##_verifier = [&](SerializationTestClass* read_obj) {assert(((SerializationTestClass*)read_obj)->PROP## == VALUE); }; 

#define SETUP_PRIVATE_PROPERTY(PROP, VALUE) \
		auto PROP##_assigner = [&]() {write_obj->set_##PROP##(VALUE); }; \
		PROP##_assigner(); \
		auto PROP##_verifier = [&](SerializationTestClass* read_obj) {assert(((SerializationTestClass*)read_obj)->get_##PROP##() == VALUE); }; 

#define VERIFY_PROPERTY(PROP, VALUE) \
		PROP##_verifier(VALUE); 

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


		
		//auto assigner = [&]() {write_obj->gvec2property_public = vec2_val; };
		//auto verifier = [&](SerializationTestClass* read_obj) {assert(((SerializationTestClass*)read_obj)->gvec2property_public == vec2_val); };

		SETUP_PUBLIC_PROPERTY(gvec2property_public, vec2_val);
		SETUP_PUBLIC_PROPERTY(gvec3property_public, vec3_val);
		SETUP_PUBLIC_PROPERTY(gvec4property_public, vec4_val);
		SETUP_PUBLIC_PROPERTY(gmat2property_public, mat2_val);
		SETUP_PUBLIC_PROPERTY(gmat3property_public, mat3_val);
		SETUP_PUBLIC_PROPERTY(gmat4property_public, mat4_val);
		SETUP_PUBLIC_PROPERTY(gboolproperty_public, bool_val);
		SETUP_PUBLIC_PROPERTY(gcharproperty_public, char_val);
		SETUP_PUBLIC_PROPERTY(gint8property_public, int8_val);
		SETUP_PUBLIC_PROPERTY(guint8property_public, uint8_val);
		SETUP_PUBLIC_PROPERTY(gint16property_public, int16_val);
		SETUP_PUBLIC_PROPERTY(guint16property_public, uint16_val);
		SETUP_PUBLIC_PROPERTY(gint32property_public, int32_val);
		SETUP_PUBLIC_PROPERTY(guint32property_public, uint32_val);
		SETUP_PUBLIC_PROPERTY(gint64property_public, int64_val);
		SETUP_PUBLIC_PROPERTY(guint64property_public, uint64_val);
		SETUP_PUBLIC_PROPERTY(gfloatproperty_public, float_val);
		SETUP_PUBLIC_PROPERTY(gstringproperty_public, string_val);
		SETUP_PUBLIC_PROPERTY(gdoubleproperty_public, double_val);
		SETUP_PUBLIC_PROPERTY(gobjectpointerproperty_public, gobject_ptr);
		SETUP_PUBLIC_PROPERTY(gcolorproperty_public, blue);
		
		/*auto gobjectpointerproperty_public_assigner = [&]() {write_obj->gobjectpointerproperty_public = gobject_ptr; };
		gobjectpointerproperty_public_assigner();
		auto gobjectpointerproperty_public_verifier = [&](SerializationTestClass* read_obj) {
			assert(((SerializationTestClass*)read_obj)->gobjectpointerproperty_public == gobject_ptr); 
		};*/

		//TODO: find a way to use assignment operator for the GObjectPointer
		//write_obj->gobjectpointerproperty_public.setValue(gobject_ptr);

		SETUP_PRIVATE_PROPERTY(gvec2property_private, vec2_val);
		SETUP_PRIVATE_PROPERTY(gvec3property_private, vec3_val);
		SETUP_PRIVATE_PROPERTY(gvec4property_private, vec4_val);
		SETUP_PRIVATE_PROPERTY(gmat2property_private, mat2_val);
		SETUP_PRIVATE_PROPERTY(gmat3property_private, mat3_val);
		SETUP_PRIVATE_PROPERTY(gmat4property_private, mat4_val);
		SETUP_PRIVATE_PROPERTY(gboolproperty_private, bool_val);
		SETUP_PRIVATE_PROPERTY(gcharproperty_private, char_val);
		SETUP_PRIVATE_PROPERTY(gint8property_private, int8_val);
		SETUP_PRIVATE_PROPERTY(guint8property_private, uint8_val);
		SETUP_PRIVATE_PROPERTY(gint16property_private, int16_val);
		SETUP_PRIVATE_PROPERTY(guint16property_private, uint16_val);
		SETUP_PRIVATE_PROPERTY(gint32property_private, int32_val);
		SETUP_PRIVATE_PROPERTY(guint32property_private, uint32_val);
		SETUP_PRIVATE_PROPERTY(gint64property_private, int64_val);
		SETUP_PRIVATE_PROPERTY(guint64property_private, uint64_val);
		SETUP_PRIVATE_PROPERTY(gfloatproperty_private, float_val);
		SETUP_PRIVATE_PROPERTY(gdoubleproperty_private, double_val);
		SETUP_PRIVATE_PROPERTY(gstringproperty_private, string_val);
		SETUP_PRIVATE_PROPERTY(gobjectpointerproperty_private, gobject_ptr);
		SETUP_PRIVATE_PROPERTY(gcolorproperty_private, blue);
		//TODO: find a way to test the below
		//write_obj->set_gobjectpointerproperty_private(gobject_ptr);

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
		//assert(((SerializationTestClass*)read_obj.get())->gvec2property_public == vec2_val);
		VERIFY_PROPERTY(gvec2property_public, ptr);
		VERIFY_PROPERTY(gvec3property_public, ptr);
		VERIFY_PROPERTY(gvec4property_public, ptr);
		VERIFY_PROPERTY(gmat2property_public, ptr);
		VERIFY_PROPERTY(gmat3property_public, ptr);
		VERIFY_PROPERTY(gmat4property_public, ptr);
		VERIFY_PROPERTY(gboolproperty_public, ptr);
		VERIFY_PROPERTY(gcharproperty_public, ptr);
		VERIFY_PROPERTY(gint8property_public, ptr);
		VERIFY_PROPERTY(guint8property_public, ptr);
		VERIFY_PROPERTY(gint16property_public, ptr);
		VERIFY_PROPERTY(guint16property_public, ptr);
		VERIFY_PROPERTY(gint32property_public, ptr);
		VERIFY_PROPERTY(guint32property_public, ptr);
		VERIFY_PROPERTY(gint64property_public, ptr);
		VERIFY_PROPERTY(guint64property_public, ptr);
		VERIFY_PROPERTY(gfloatproperty_public, ptr);
		VERIFY_PROPERTY(gdoubleproperty_public, ptr);
		VERIFY_PROPERTY(gstringproperty_public, ptr);
		//TODO: find a way to use assignment operator for the GObjectPointer
		//assert(ptr->gobjectpointerproperty_public.getValue() == gobject_ptr);
		VERIFY_PROPERTY(gobjectpointerproperty_public, ptr);
		VERIFY_PROPERTY(gcolorproperty_public, ptr);

		VERIFY_PROPERTY(gvec2property_private, ptr);
		VERIFY_PROPERTY(gvec3property_private, ptr);
		VERIFY_PROPERTY(gvec4property_private, ptr);
		VERIFY_PROPERTY(gmat2property_private, ptr);
		VERIFY_PROPERTY(gmat3property_private, ptr);
		VERIFY_PROPERTY(gmat4property_private, ptr);
		VERIFY_PROPERTY(gboolproperty_private, ptr);
		VERIFY_PROPERTY(gcharproperty_private, ptr);
		VERIFY_PROPERTY(gint8property_private, ptr);
		VERIFY_PROPERTY(guint8property_private, ptr);
		VERIFY_PROPERTY(gint16property_private, ptr);
		VERIFY_PROPERTY(guint16property_private, ptr);
		VERIFY_PROPERTY(gint32property_private, ptr);
		VERIFY_PROPERTY(guint32property_private, ptr);
		VERIFY_PROPERTY(gint64property_private, ptr);
		VERIFY_PROPERTY(guint64property_private, ptr);
		VERIFY_PROPERTY(gfloatproperty_private, ptr);
		VERIFY_PROPERTY(gdoubleproperty_private, ptr);
		VERIFY_PROPERTY(gstringproperty_private, ptr);
		VERIFY_PROPERTY(gobjectpointerproperty_private, ptr);
		VERIFY_PROPERTY(gcolorproperty_private, ptr);
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