[中文介绍](Doc/Introduction/中文/README.md)

# Niflect

The C++ native-style type reflection framework.

**This is a concept currently in the experimental stage.**

Before you begin reading, it’s important to ensure that you have the necessary experience to fully benefit from this article and to avoid wasting your time. Specifically, you should :

- **Understand the Necessity of C++ Reflection**: It is essential that you grasp why it is important to study C++ reflection.

Additionally, you should possess at least one of the following qualifications:

- **5+ years of C++ Experience**: Ideally, you have extensive experience in using C++.
- **Experience in Large Software Architecture**: Relevant experience in designing or working with large software systems.
- **Extensive Coding Background**: Have written over 300,000 lines of code.

If you meet the requirements above, feel free to continue reading to explore the Niflect framework and its potential applications.

## [Experiment 1] The Reflection Code Gen Tool

The reflection code gen tool in Niflect, is called NiflectGenTool. Its usage steps are as follows :

### Step 1

An example header

```C++
#pragma once
#include "MyClass_gen.h"

NIF_T()
class CMyClass
{
	GENERATED_BODY()
public:
	CMyClass()
		: m_bool_1(false)
	{
	}
	void InitForTest()
	{
		m_std_string_0 = "My std string";
		m_bool_1 = true;
	}
	bool operator==(const CMyClass& rhs) const
	{
		return m_std_string_0 == rhs.m_std_string_0
			&& m_bool_1 == rhs.m_bool_1
			;
	}

private:
	NIF_F()
	std::string m_std_string_0;
	NIF_F()
	bool m_bool_1;
};
```

The macro tags like `NIF_T` are tags for reflection code gen.

### Step 2

Execute NiflectGenTool to generate reflection code, here is a batch script example, like

```bash
@..\ThirdParty\NiflectGenTool\bin\NiflectGenTool.exe ^
-n MyApp ^
-h ../MyClass.h ^
-a ../ThirdParty/Niflect/include/Niflect/CommonlyUsed/DefaultAccessorSetting.h ^
-I ../ThirdParty/Niflect/include ^
-I ../ ^
-p NiflectGenerated 
```

Then the reflection code will be generated

![1732206465218](../Wishing/Doc/Introduction/1732206465218.png)

About the **DefaultAccessorSetting.h** appears in the commandline args

This is one of the Niflect's feature, it's a header, called Accessor Setting Header, for specifying type bindings in a way of C++ native style.

Here is a simplified version of the header

```C++
namespace DefaultAccessorSetting
{
	using namespace NiflectAccessorSetting;
	using namespace Niflect;

	NIF_AS_A() TSetting<CBoolAccessor, bool>;
	NIF_AS_A() TSetting<CSTLStringAccessor, std::string>;
}
```

This header can be interpreted by code completion tools.

Of course, users can implement settings like this to bind any `Accessor` for custom field serialization.

### Step 3

To save and load the example instance of `CMyClass ` with reflection info.

```C++
#include "MyClass.h"
#include "MyApp/ModuleReg/MyApp_private.h"
#include "Niflect/NiflectTable.h"
#include "Niflect/Serialization/JsonFormat.h"

int main()
{
	using namespace Niflect;

	CNiflectTable table;

	{
		Niflect::GeneratedInitialReg(&table);
		table.InitTypesLayout();
	}

	CRwNode rw;

	{
		CMyClass srcData;
		srcData.InitForTest();
		
		{
			// Save the 'srcData' instance to the 'rw' node
			auto type = StaticGetType<CMyClass>();
			type->SaveInstanceToRwNode(&srcData, &rw);
		}

		TSharedPtr<void*> dummy;
		{
			auto type = StaticGetType<CMyClass>();
			// Create CMyClass dummy instance from reflected type info.
			dummy = type->MakeSharedInstance<void*>();
			// Load the dummy instance from the 'rw' node
			type->LoadInstanceFromRwNode(dummy.Get(), &rw);
		}
		
		// Copy the loaded dummy instance to 'dstData'
		CMyClass dstData = *reinterpret_cast<CMyClass*>(dummy.Get());
		// Assert that the saving and laoding were successful
		ASSERT(srcData == dstData);
	}

	// Serialize 'rw' to JSON format and then print the result
	CStringStream ss;
	CJsonFormat::Write(&rw, ss);
	printf("%s\n", ss.str().c_str());

	return 0;
}
```

and the result

![1732212533978](../Wishing/Doc/Introduction/1732212533978.png)

## [Experiment 2] Being C++ Native

C++ programmers may need to :

- Modify a code generation tool for C++ type bindings or for types in other languages, such as C#.
- Parse various string formats to retrieve metadata about types and fields.

This may be necessary because the C++ reflection framework is not fully native.

Due to this, being native is an essential requirement for C++ programmers.

Here is an example that illustrates what it means to be C++ native using Niflect :

```C++
namespace MyScope
{
	static CMyFieldMeta MyGetNata()
	{
		return CMyFieldMeta().SetFriendlyName("Being native is all that matters");
	}

	class CMyClassNata : public Niflect::CNata
	{
		typedef CMyClassNata CThis;
	public:
		CThis& SetCategory(const Niflect::CString& name)
		{
			m_categoryName = name;
			return *this;
		}
		Niflect::CString m_categoryName;
	};

	NIF_T(CMyClassNata().SetCategory("My Default Category"))
	class CEngineObject
	{
		GENERATED_BODY()
	public:
		NIF_F(CMyFieldMeta()
			.SetFriendlyName("This is a nata in multi lines"))
		float m_float_0;
		NIF_F(MyGetNata())
		Niflect::CString m_string_2;
		NIF_F()
		std::string m_std_string_3;
		NIF_F()
		std::vector<float> m_std_array_float_4;
		NIF_F()
		std::vector<std::vector<float> > m_std_array_array_float_5;
		NIF_F()
		std::vector<bool> m_std_bits_6;
		NIF_F()
		std::map<std::string, std::vector<float> > m_std_map_string_array_float_7;
	};
}
```

### Nata

The Nata, such as `CMyClassNata`, refers to native-style type/field metadata. It can be interpreted by code completion tools, allowing users to efficiently write metadata code without the need to remember which properties are supported by the framework.

It's remarkable that users can wrap the Nata specification into other methods, such as `MyGetNata()`.

### Nested Templates & Specialized Templates

The nested templates, such as `std::vector<float>`, and the specialized templates, like `std::vector<bool>`, can be bound by specifying the settings in an accessor settings header, as shown below

```C++
using namespace NiflectAccessorSetting;

template <typename TInstance>
NIF_AS_A() TSetting<TSTLBitsArrayAccessor<TInstance>, std::vector<bool> >;

template <typename TInstance, typename T>
NIF_AS_A() TSetting<TSTLArrayAccessor<TInstance>, std::vector<T> >;

template <typename TInstance, typename T0, typename T1>
NIF_AS_A() TSetting<TSTLMapAccessor<TInstance>, std::map<T0, T1>, std::pair<T0, T1> >;
```

Again, the Accessor Setting Headers allows users to bind their `Accessor` for their own custom serialization without needing to modify code of reflection code gen tool like NiflectGenTool.

More importantly, this type binding method is entirely in a C++ native style.

## [Experiment 3] Serialization for A Pointer Type

C++ reflection frameworks we have explored typically face limitations when it comes to serializing pointers. They

- Cannot serialize pointers at all
- Or only support specific types of pointers.

This limitation is understandable, as general-purpose frameworks must not have enough information to create or manage custom pointer types effectively.

However, with Niflect, users have the capability to serialize any pointer type by implementing a custom accessor setting. For example :

```C++
namespace SampleAccessorSetting
{
	NIF_AS_A() TSetting<CMyResourceAccessor, CMyResource*>;
}
```

In this snippet :

- `CMyResourceAccessor` serves as the accessor for serializing pointers of type `CMyResource`.
- `CMyResource` is the pointer type used to demonstrate pointer serialization.

The implementation of the `CMyResourceAccessor`:

```C++
class CMyResourceAccessor : public Niflect::CAccessor
{
	using MyPtr = CMyResource*;
protected:
	virtual bool SaveInstanceImpl(const AddrType base, CRwNode* rw) const
	{
		const auto& instance = *static_cast<const MyPtr*>(base);
		AddRwString(rw, "Path", instance->m_path.c_str());
		return true;
	}
	virtual bool LoadInstanceImpl(AddrType base, const CRwNode* rw) const
	{
		auto& instance = *static_cast<MyPtr*>(base);
		auto path = FindRwString(rw, "Path");
		instance = g_factory.FindOrAddResource(path.c_str());
		return true;
	}
};
```

In this setup, the example class type with a pointer field :

```C++
class CMyResource
{
public:
	CMyResource(const std::string& path)
		: m_path(path)
	{
	}
	std::string m_path;
};

class CMyFactory
{
public:
	CMyResource* FindOrAddResource(const std::string& path)
	{
		auto ret = m_mapPathToResource.insert({ path, NULL });
		if (ret.second)
			ret.first->second = std::make_shared<CMyResource>(path);
		return ret.first->second.get();
	}

	std::map<std::string, std::shared_ptr<CMyResource> > m_mapPathToResource;
};

extern CMyFactory g_factory;

NIF_T()
class CMyClass
{
	GENERATED_BODY()
public:
	CMyClass()
		: m_resource_0(NULL)
	{
	}
	void InitForTest()
	{
		m_resource_0 = g_factory.FindOrAddResource("/Dir/Image.bmp");
	}
	bool operator==(const CMyClass& rhs) const
	{
		return m_resource_0 == rhs.m_resource_0
			;
	}

public:
	NIF_F()
	CMyResource* m_resource_0;
};

```

Finally, do save and load `CMyClass` instance like the Step 3 of Experiment 1

![1732372828784](../Wishing/Doc/Introduction/1732372828784.png)

## The Repo of The Experiments Above

https://github.com/sainimu78/NiflectSample

## Summary

Being C++ native is all that matters.

### Features

- C++ native-style type binding through Accessor Setting Headers
  - Extremely easy to implement custom type/field serialization without modifying the code generation tool
- C++ native-style type/field metadata specification
  - Users can write metadata code in a native C++ style
- libclang-based parser, meaning that adapting to a coding standard is not required
  - Eg. support for type aliases defined by `typedef` or `using`
  - Eg. support for namespace scope or nested namespace scopes
- Truly non-intrusive reflection implementation
  - No base type requirement for types being reflected.
  - Macro tags like `NIF_T` are empty macros, while `GENERATED_BODY` consists solely of `friend` declarations, without any definitions for fields or methods. (These macro tags can be easily renamed using `#define` as needed.)
- Extensible C++ native serialization method utilizing `RwTree`
  - Data is saved into the unified tree-like structure of `CRwNode`, allowing serialization into any other desired format.

### About the Name "Niflect"

By combining these elements :

- C++ `N`ative
- `N`on-`i`ntrusive 
- C++ Re`flect`ion

the name "Niflect" is formed.