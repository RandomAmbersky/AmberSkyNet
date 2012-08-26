// [1258] В детстве я нередко сочинял заведомый вздор только для того,
// чтобы вызвать удивление окружающих.
//  Ч. Дарвин

#ifndef _I_ASN_GENERATOR_PLUGIN_H
#define _I_ASN_GENERATOR_PLUGIN_H

#include "IEngine.h"

struct IAsnGeneratorInfo
{
	const char* name;         //имя загрузочного модуля
        const char* type;             //поддерживаемые типы генерации экземлпяров классов
        const char* author;           //автор плагина
        const char* description;      //описание плагина
        const char* plugin_version;   //версия плагина
        const char* required_version; //требуемая версия движка, не ниже   //TODO

};

struct  IAsnGenerator
{
public:
//    virtual IAsnGenetaror() {Engine=NULL;};
//    virtual ~IAsnGenerator() { };

// возвращаем экземпляр класса по типу с именем Type
// virtual void *GetClass(const char* Type)=0;

    virtual IBaseObject* GetObject(const char* Type)=0;

// передаём указатель на Engine
    virtual void Initialize(IEngine *vEngine) {Engine=vEngine;}
// получаем информацию о плагине
    virtual IAsnGeneratorInfo GetInfo() {return GeneratorInfo;}

protected:
// информация о плагине. Заполняется в конструкторе класса
IAsnGeneratorInfo GeneratorInfo;
// указатель на главный класс, хранящийся в плагине. Нужен для генерации экземпляров
//классов, которым при создании требуется указатель на Engine.
IEngine *Engine;
};

#endif
