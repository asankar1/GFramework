#pragma once
#include <map>
#include <mutex>
#include <vector>
#include <istream>
#include <ostream>
#include <GFramework/GReflection/GReflectionHelpers.h>

namespace GFramework
{
	class GObject;
	class GMetaproperty;

	/** \var typedef std::shared_ptr<std::istream> IStreamSharedPtr;
	*	\brief shared pointer type for the std::istream
	*/
	typedef std::shared_ptr<std::istream> IStreamSharedPtr;

	/** \var typedef std::shared_ptr<std::ostream> IStreamSharedPtr;
	*	\brief shared pointer type for the std::ostream
	*/
	typedef std::shared_ptr<std::ostream> OStreamSharedPtr;

	class GFRAMEWORK_API GSerializer
	{
	public:
		virtual ~GSerializer();
		virtual bool open(OStreamSharedPtr _stream);
		OStreamSharedPtr getStream();
		virtual void close();
		/*template<typename T>
		GSerializer& operator<<(T& prop)
		{
			static_assert(	std::is_base_of<GPropertyInterface, std::remove_reference<std::remove_cv<T>::type>::type>::value ||
							std::is_base_of<GObject, std::remove_reference<std::remove_cv<T>::type>::type>::value, "Serializer can serialize only GObject or GPropertyInterface!");
			write(prop);
		}

		template<typename T>
		GSerializer& operator<<(T* prop)
		{
			static_assert(std::is_base_of<GPropertyInterface, std::remove_reference<std::remove_cv<T>::type>::type>::value ||
				std::is_base_of<GObject, std::remove_reference<std::remove_cv<T>::type>::type>::value, "Serializer can serialize only GObject or GPropertyInterface!");
			write(prop);
		}*/

		/*virtual GSerializer& operator<<(GObject& _obj) = 0;
		virtual GSerializer& operator<<(GObject* _obj) = 0;*/
		GSerializer& operator<<(const GPropertyInterface& prop);

		template<class T>
		GSerializer& operator<<(std::shared_ptr<T> &prop)
		{
			static_assert(std::is_base_of<GObject,T>::value,"Serializer can only serialized shared pointer objects that are derived from GObjects");
			//TODO: Currently all GObjects are added as reference provider, even they are not referenced by any other GObjects,
			//see if this can be optimized
			addReferenceProvider(std::static_pointer_cast<GObject>(prop));
			return write(std::static_pointer_cast<GObject>(prop));
		}

		GSerializer& operator<<(GObjectSharedPtr& prop)
		{
			//TODO: Currently all GObjects are added as reference provider, even they are not referenced by any other GObjects,
			//see if this can be optimized
			addReferenceProvider(prop);
			return write(prop);
		}
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) = 0;


	protected:
		virtual GSerializer& write(const GPropertyInterface& property)
		{
			return *this;
		}

		virtual GSerializer& write(const GObjectSharedPtr& _obj)
		{
			return *this;
		}

		static void addReferenceProvider(GObjectSharedPtr obj);

		class PostProcessor
		{
		public:
			void process(GSerializer& serializer);
			void addPointer(const GPointerPropertyInterface* pointer_prop);
			~PostProcessor();
		private:
			std::map<unsigned int, std::vector<GObjectSharedPtr> > reference_providers;
			PostProcessor* childPP = nullptr;
			bool isProcessing = false;
		};
		PostProcessor postProcessor;
		//TODO: See if the below can be non static
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<std::shared_ptr<GPointerPropertyInterface> > > reference_seekers;
		static std::map<unsigned int, std::vector<GObjectSharedPtr> > reference_providers;
		OStreamSharedPtr stream;
	};

#if 0
	class GFRAMEWORK_API GBinarySerializer : public GSerializer
	{
	public:
		virtual ~GBinarySerializer();
		virtual bool open(OStreamSharedPtr _stream) override;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) override;
		/*virtual GSerializer& operator<<(GObject& _obj) override;
		virtual GSerializer& operator<<(GObject* _obj) override;*/
	};

	class GFRAMEWORK_API GTextSerializer : public GSerializer
	{
	public:
		GTextSerializer();
		virtual ~GTextSerializer();
		virtual bool open(OStreamSharedPtr _stream) override;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) override;
		/*virtual GSerializer& operator<<(GObject& _obj) override;
		virtual GSerializer& operator<<(GObject* _obj) override;*/
	protected:
		const char* objectDelimiter;
	};
#endif
	class GFRAMEWORK_API GDeserializer
	{
	public:
		class GObjectSharedPtrSetter
		{
		public:
			GObjectSharedPtrSetter(GObject* _obj) : obj(_obj) {}
			virtual void setValue(std::shared_ptr<GObject>& value) = 0;
		protected:
			GObject* obj;
		};

		template<class SETTER>
		class NonPublicGObjectSharedPtrSetter : public GObjectSharedPtrSetter
		{
		public:
			NonPublicGObjectSharedPtrSetter(GObject* _obj, SETTER _setter) : GObjectSharedPtrSetter(_obj), setter(_setter){}
			virtual void setValue(std::shared_ptr<GObject>& value) override {
				//TODO: avoid function call and deduce the type
				auto t = find_type(setter);
				using arg_type = typename std::remove_reference<ArgType<SETTER, 1> >::type;//actual type which may be derived from GObject
				using class_member_t = decltype(t);
				((dynamic_cast<typename class_member_t::class_type*>(obj))->*setter)(std::dynamic_pointer_cast<typename arg_type::element_type>(value));
			}
		protected:
			SETTER setter;
		};

		template<class SETTER>
		class NonPublicGObjectPointerPropertySetter : public GObjectSharedPtrSetter
		{
		public:
			NonPublicGObjectPointerPropertySetter(GObject* _obj, SETTER _setter) : GObjectSharedPtrSetter(_obj), setter(_setter) {}
			virtual void setValue(std::shared_ptr<GObject>& value) override {
				//TODO: avoid function call and deduce the type
				auto t = find_type(setter);
				using arg_type = typename std::remove_reference<ArgType<SETTER, 1> >::type;//actual type which may be derived from GObject
				using class_member_t = decltype(t);
				((dynamic_cast<typename class_member_t::class_type*>(obj))->*setter)(value);
			}
		protected:
			SETTER setter;
		};

		

	public:
		virtual ~GDeserializer();
		virtual bool open(IStreamSharedPtr _stream);
		IStreamSharedPtr getStream();
		virtual void close();
		//virtual GDeserializer& operator>>(GObject** _obj) = 0;
		virtual bool readMetaProperty(GObjectSharedPtr _obj, GMetaproperty* property) = 0;
		template<class SETTER>
		static void addReferenceSeeker(unsigned int _object_id, GObject* _obj, SETTER setter)
		{
			using arg_type = typename std::remove_reference<ArgType<SETTER, 1> >::type;//actual type which may be derived from GObject
			using setter_type = typename std::conditional<std::is_same<arg_type, GObjectPointerProperty>::value, NonPublicGObjectPointerPropertySetter<SETTER>, NonPublicGObjectSharedPtrSetter<SETTER>>::type;
			std::lock_guard<std::mutex> lk(reference_seeker_mutex);
			(reference_seekers[_object_id]).push_back(std::make_shared<setter_type>(_obj, setter));
		}
		static void addReferenceSeeker(unsigned int _object_id, GObjectSharedPtr* _seeking_object);
		static void addReferenceProviders(unsigned int _object_id, GObjectSharedPtr _providing_object);
		void resolveDependencies();
		void setObject_id(GObjectSharedPtr _obj, uint32 id);

		GDeserializer& operator>>(GPropertyInterface& prop)
		{
			read(prop);
			/*if (prop.isGObjectPointer())
			{
				auto pointer_prop = dynamic_cast<GPointerPropertyInterface*>(&prop);
				//if (pointer_prop->getObjectId())
				{
					{
						auto itr = reference_seekers.find(pointer_prop->getObjectId());
						if (itr == reference_seekers.end())
						{
							reference_seekers.insert({ pointer_prop->getObjectId(), std::vector<std::shared_ptr<GPointerPropertyInterface> >({ pointer_prop }) });
						}
						else
						{
							itr->second.push_back(pointer_prop);
						}
					}
				}
			}*/

			return *this;
		}

		GDeserializer& operator>>(GObjectSharedPtr* _obj);

	protected:
		virtual GDeserializer& read(GPropertyInterface& property)
		{
			return *this;
		}

		virtual GDeserializer& read(GObjectSharedPtr* _obj)
		{
			return *this;
		}
		//TODO: See if the below can be non static
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		//static std::map<unsigned int, std::vector<std::shared_ptr<GPointerPropertyInterface> > > reference_seekers;
		static std::map<unsigned int, std::vector<std::shared_ptr<GDeserializer::GObjectSharedPtrSetter> > > reference_seekers;
		static std::map<unsigned int, GObjectSharedPtr> reference_providers;
		IStreamSharedPtr stream;
	};
#if 0
	class GFRAMEWORK_API GBinaryDeSerializer : public GDeserializer
	{
	public:
		virtual ~GBinaryDeSerializer();
		virtual bool open(IStreamSharedPtr _stream) override;
		//virtual GDeserializer& operator >> (GObject** _obj) override;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) override;
	};

	class GFRAMEWORK_API GTextDeSerializer : public GDeserializer
	{
	public:
		GTextDeSerializer();
		virtual ~GTextDeSerializer();
		virtual bool open(IStreamSharedPtr _stream) override;
		//virtual GDeserializer& operator >> (GObject** _obj) override;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) override;
	protected:
		const char* objectDelimiter;
		virtual std::pair<std::string, std::string> parseProperty(std::string line);
	};
#endif
}