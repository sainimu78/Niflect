#pragma once

//Ҫ���ģ���Accessor���ͱ���ʹ��NIF_T���
//2024.08.21, �ֹ滮������Ҫ��, �� Accessor ��Ϊһ�������ģ����
//#define TAGGED_REQUIREMENT_OF_NON_TEMPLATE_ACCESSOR_TYPE

//���û��ṩ CCompoundField �ඨ��
//2024.08.23, �� NiflectAccessor.h ���Դ� CCompoundAccessor, ��Ϊ���಻��Ҫ�Զ���, �Ǹ��Ͻṹ(class, struct)�ı�Ҫ Accessor ����
//#define USER_PROVIDED_CCOMPONENTFIELD_CLASS_DEFINITION

//���ɵ�SplittedModuleReg�е���TypeRegʱ��Ҫר��namespace
//2024.08.23, ��Ϊ�򻯲���Ҫnamespace
//#define INVOKE_TYPEREG_WITH_NAMESPACE

//������ʵ��ģ��ʵ����Scope̫����, ����Ҫ����Detail��Cursor, ��ȷ��BuildSubcursor�ܷ���ȷ����namespace����Ƕ�׵�Scope, ��˷���
//#define TEMPLATE_INSTANCE_SCOPE

//����ΪScope
//#define CLASS_TYPE_SCOPE

//�� TaggedType �� IndexedRoot ��ʼ������
//#define TODO_SIMPLIFY_TAGGED_TYPE_INDEXED_ROOT_INITIALIZATION

//ʵ��ö�ٵĴ�����������, ��ר�ŵ� AccessorBinding, CodeWriter
#define TODO_ENUM_CODE_GEN_PIPELINE

//BindingType �ظ�ָ�����
//2024.10.18, ʵ�����Ϊ, δ�ҵ����е�ͨ�ð취ʵ���ظ�ָ�� BindingType ���. �Է�ģ������߿�����, ������ģ�����, �޷�ʵ�����ֲ����ػ������
//	�ƻ�ͨ�����ɼ���ظ�ָ���Ĵ���ʵ����ȷ�Լ��, �����ɻ��� typeid(MyType).hash_code() �ļ�����
//#define BINDING_TYPE_DUPLICATION_VERIFICATION

//��ʱ���Կ���
//#define DEBUG_FOR_TYPE_REG

//ԭBindingSetting��ΪAccessorSetting
#define ACCESSOR_SETTING_ABCD

//���� map ����, ���� stack, ��д��ԭ���� 1. linux ��ͨ�� loc �Ҳ���; 2. ��ȷ�����ǩ�÷�, һ�����������Ͷ���֮ǰ���һ��
//2024.11.15, ����ͨ�� loc �Ҳ���, �������� linux �� libclang �Ļ���, ����������ʹ����Ĭ�ϵ� libclang 20, Ӧʹ�� libclang 17
//	���˵���Ҫԭ����, �������ú��ʵ�ַ���, �޷���ȡ��ȷ�ĺ��ǩ token, ��δ�ƻ����� 20 ������ȡ��ȷ�ĺ��ǩ token ����, �����ʹ��δ���ú�Ļ���ʵ��
//#define SIMPLIFIED_MACRO_CURSOR_FINDING

//��ֲ AccessMethod ��صĴ���
//#define PORTING_ACCESS_METHOD

//��ֲ Getter Setter DefaultValue �ؼ���
//#define PORTING_GETTER_SETTER_DEFAULTVALUE

//���� ACCESSMETHOD ���ǩ, �ɼ���һ��ָ����Ӧ Getter / Setter
#define DEPRECATED_ACCESSMETHOD_MACRO_TAG