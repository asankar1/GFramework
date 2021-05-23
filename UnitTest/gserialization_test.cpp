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

struct Color
{
	Color() {}
	Color(int8 _r, int8 _g, int8 _b, int8 _a) : r(_r), g(_g), b(_b), a(_a) {}
	int8 r=0, g=0, b=0, a=0;
};
namespace GFramework {
	class GColorProperty : public GPropertyInterface
	{
	public:
		GColorProperty(Color v = Color()) : value(v) {}

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
			os << (unsigned short)value.r << " " << (unsigned short)value.g << " " << (unsigned short)value.b << " " << (unsigned short)value.a << " ";
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
	GInt16troperty x;
	GInt16troperty y;
	virtual void initialize(){}
	virtual GMetaclass* getMetaclass() const
	{
		return GMetaNamespaceList::_global().getMetaclass("Circle");
	}
	int32 getRadius() const { return radius.getValue(); }
	void setRadius(int32 v) { radius.setValue(v); }
	void calculateBBox() {
		boundingBox.x = x.getValue();
		boundingBox.y = y.getValue();
		boundingBox.setSize(2 * radius.getValue());
	}
	Square boundingBox;
	GColorProperty color;
	GObjectPointerProperty parent;
private:
	GInt32Property radius;

};

BEGIN_DEFINE_META(Circle)
	GMetaNamespaceList::_global()
	.define<Circle>("Circle")
	.property("radius", &Circle::getRadius, &Circle::setRadius)
	.property("x", &Circle::x)
	.property("y", &Circle::y)
	.property("Color", &Circle::color)
	.property("parent", &Circle::parent)
	/*.property("boundingbox", &Circle::boundingBox)*/;
END_DEFINE_META(Circle)

/*BEGIN_DEFINE_META(Square)
	GMetaNamespaceList::_global()
	.define<Square>("Square")
	.property("size", &Square::getSize, &Square::setSize)
	.property("x", &Square::x)
	.property("y", &Square::y);
END_DEFINE_META(Square)*/


void run_serialization_testcases()
{
	cout << endl << "Starting test cases for 'Serialization'..." << endl << endl;
	
/*	NodeSharedPtr nullparent(NULL);
	NodeSharedPtr osphere1_ptr = make_shared<sphere>("sphere1", nullparent, 25);
	sphere osphere2("sphere2", osphere1_ptr, 15);
*/
#if 1
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

		parent_circle->parent.setValue(grand_parent_circle);
		ic1.parent.setValue( parent_circle);

		Square is1;
		is1.setSize(17);
		is1.x = 93;
		is1.y = 74;

		GStringSerializer ts;
		ts.open(out_text);
		GInt32Property i = 4;
		GInt16troperty s = 2;
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
		GInt16troperty ss = 2;
		GColorProperty cc(Color(1, 2, 3, 4));
		GObjectSharedPtr op1 = nullptr;
		GObject* op2 = nullptr;
		tds >> ss >> ii >> ss >> cc;
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