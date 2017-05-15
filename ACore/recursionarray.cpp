#include "recursionarray.h"
#include <QTextStream>
using namespace ACore;
RecursionArray::RecursionArray()
{

}

RecursionArray::RecursionArray(std::initializer_list<RecVal> list)
{
    for(auto i=list.begin();i!=list.end();++i)
    {
        this->operator []((*i).name) = (*i).val;
    }
}
QString ACore::DeleteQuotes(QString str)
{
    if(str[0]=='"' && str[str.size()-1]=='"')
    return str.remove(0,1).remove(str.size()-2,1);
    else return str;
}
QString ACore::DeleteSpaceStart(QString str)
{
    int deleteSize=0;
    for(int i=0;str[i]==' ';i++) deleteSize++;
    return str.remove(0,deleteSize);
}
QMap<QString, QVariant> RecursionArray::_fromPostGetFormat(const QString post)
{
    QMap<QString,QVariant> result;
    QStringList argList = post.split("&");
    if(argList.isEmpty()) argList << post;
    for(int i=0;i<argList.size();i++)
    {
        QString str = argList.value(i);
        QStringList values = str.split("=");
         if(!values.value(0).isEmpty() && !values.value(1).isEmpty()) result[values.value(0)]=values.value(1);
    }
    return result;
}

QString RecursionArray::toPost()
{
    QString result;
    QStringList keyd=keys();
    for(int i=0;i<size();i++)
    {
        if(i==0) result+=keyd.value(i)+"="+VariantToString(value(keyd.value(i)));
        else result+="&"+keyd.value(i)+"="+VariantToString(value(keyd.value(i)));
    }
    return result;
}
QString RecursionArray::toHtml()
{
    return _toHTMLTegsFormat(*this);
}
void RecursionArray::fromHtml(const QString value)
{
    _fromHTMLTegsFormat(value,false);
}
void RecursionArray::fromArcan(const QByteArray value)
{
    operator=(_fromArcanFromat(value));
}
void RecursionArray::fromPost(QString post)
{
    operator=(_fromPostGetFormat(post));
}
void RecursionArray::fromCfg(const QString yum)
{
    _fromCfgFormat(yum,true);
}
void RecursionArray::fromYum(const QString yum)
{
    operator=(_fromYumFormat(yum));
}



QString RecursionArray::_toHTMLTegsFormat(RecursionArray Map)
{
    QString ReturnValue;
    for(auto i = Map.begin();i!=Map.end();++i)
    {
        ReturnValue+="<"+i.key()+">";
        if(i.value().type() == QVariant::Map){
        QString tmp= _toHTMLTegsFormat(i.value().toMap());
        ReturnValue+=tmp;}
        else ReturnValue+=VariantToString(i.value());
        ReturnValue+="</"+i.key()+">";
    }
    return ReturnValue;
}
QString RecursionArray::_toYUMFormat(RecursionArray Map,QString Tabulator)
{
    QString ReturnValue;
    for(auto i = Map.begin();i!=Map.end();++i)
    {
        ReturnValue+="\n"+Tabulator+i.key()+":";
        QString tmp= _toYUMFormat(i.value().toMap(),Tabulator+" ");
        if(tmp.isEmpty()) ReturnValue+=VariantToString(i.value());
        else ReturnValue+=tmp;
    }
    return ReturnValue;
}
QString RecursionArray::_toArcanFormat(RecursionArray Map)
{
    QString ReturnValue;
    for(auto i = Map.begin();i!=Map.end();++i)
    {
        ReturnValue+=QString(i.key()).replace("[","\\[").replace("]","\\]")+"[";
        if(i.value().type()==QVariant::Map)
        ReturnValue+=_toArcanFormat(i.value().toMap())+"]";
        else
        ReturnValue+=VariantToString(i.value()).replace("[","\\[").replace("]","\\]")+"]";
    }
    return ReturnValue;
}
QByteArray RecursionArray::_toCFGFormat(RecursionArray Map)
{
    bool isStart=true;
    QByteArray ReturnValue;
    for(auto i = Map.begin();i!=Map.end();i++)
    {
        QString TypeValue;
        QVariant Value=i.value();
        if(Value.type()==QVariant::String) TypeValue="S";
        else if(Value.type()==QVariant::Int) TypeValue="I";
        else if(Value.type()==QVariant::Double) TypeValue="D";
        else if(Value.type()==QVariant::Bool) TypeValue="B";
        else if(Value.type()==QVariant::Map)
        {
            QByteArray tmp= _toCFGFormat(Value.toMap());
            if(!tmp.isEmpty()){
            ReturnValue.append('\n').append(i.key().toUtf8()).append(" {\n").append(tmp).append("\n}");
            }
            continue;
        }
        else
        {
            continue;
        }
        if(!isStart)ReturnValue.append('\n').append(TypeValue.toUtf8()).append(':').append(i.key().toUtf8()).append('=');
        else ReturnValue.append(TypeValue).append(":").append(i.key()).append("=");
        ReturnValue.append(VariantToString(i.value()).toUtf8());
        if(isStart) isStart=false;
    }
    return ReturnValue;
}
QString RecursionArray::printList(QList<QVariant> List)
{
    QString ReturnValue="[";
    QVariant Value0=List.value(0);
    if(Value0.type()==QVariant::String) if(!Value0.toString().isEmpty()) ReturnValue+=Value0.toString();
    for(int i=1;i<List.size();i++)
    {
        QVariant Value=List.value(i);
        if(Value.type()==QVariant::String) if(!Value.toString().isEmpty())
        ReturnValue+=","+Value.toString();
    }
    ReturnValue+="]";
    return ReturnValue;
}
QString RecursionArray::printMap(RecursionArray Map,QString NameMap,QString Tabulator)
{
    QString ReturnValue;
    int i=0;
    QList<QString> keys=Map.keys();
    if(keys.size()<=0) return "";
    ReturnValue+=Tabulator+"QMap("+NameMap+")\n"+Tabulator+"{\n";
    while(i<keys.size())
    {
        QString NameKey=keys.value(i);
        QVariant tmp=Map.value(NameKey);
        if(tmp.type()!=QVariant::Map) ReturnValue+=Tabulator+"   ["+NameKey+"] = ";
        if(tmp.type()==QVariant::String) ReturnValue+= Map.value(NameKey).toString();
        else if(tmp.type()==QVariant::ByteArray) ReturnValue+= QString(Map.value(NameKey).toByteArray());
        else if(tmp.type()==QVariant::List) ReturnValue+=printList(Map.value(NameKey).toList());
        else if(tmp.type()==QVariant::Int) ReturnValue+=QString::number(Map.value(NameKey).toInt());
        else if(tmp.type()==QVariant::Double) ReturnValue+=QString::number(Map.value(NameKey).toDouble());
        else if(tmp.type()==QVariant::Bool)
        {
            if(tmp.toBool()) ReturnValue+="true";
            else ReturnValue+="false";
        }
        else if(tmp.type()==QVariant::Map) {
            ReturnValue+=printMap(Map.value(NameKey).toMap(),NameKey,Tabulator+"   ");
        }
        else ReturnValue+="Unknown()";
        ReturnValue+="\n";
        i++;
    }
    ReturnValue+=Tabulator+"}";
    return ReturnValue;
}
QString RecursionArray::VariantToString(QVariant tmp)
{
    if(tmp.type()==QVariant::String) return  tmp.toString();
    else if(tmp.type()==QVariant::ByteArray) return QString(tmp.toByteArray());
    else if(tmp.type()==QVariant::List) return printList(tmp.toList());
    else if(tmp.type()==QVariant::Int) return QString::number(tmp.toInt());
    else if(tmp.type()==QVariant::Double) return QString::number(tmp.toDouble());
    else if(tmp.type()==QVariant::Bool)
    {
        if(tmp.toBool()) return "true";
        else return "false";
    }
    else if(tmp.type()==QVariant::Map) {
        return printMap(tmp.toMap());
    }
    else return "Unknown()";
}
QMap<QString,QVariant> RecursionArray::_fromYumFormat(QString yum,QString level, bool isReturn)
{
    QStringList fromBR=yum.split("\n");
    QMap<QString,QVariant> ReturnMap;
    for(int i=0;i<fromBR.size();i++)
    {
        QString ValueString=fromBR.value(i);
        if(level.size()>0) ValueString.remove(0,level.size());
        if(ValueString.toLocal8Bit()[0]=='#') continue;
        int position=ValueString.indexOf(":");
        if(position<=0) continue;
        QString NameValue=ValueString.mid(0,position);
        QString Value=ValueString.mid(position+1);
        if(!Value.isEmpty())
        {
            if(!isReturn)
                operator [](NameValue)=Value;
            else
                ReturnMap[NameValue]=Value;
        }
        else
        {
            int unusedsize=0;bool stop=true; QString sendString,nextLevel;
            nextLevel=level+ " ";
            while (stop) {

                if(fromBR.value(i+unusedsize+1).mid(0,nextLevel.size())==nextLevel){
                    unusedsize++;
                    sendString+=fromBR.value(i+unusedsize)+"\n";
                }
                else stop=false;
            }
            QMap<QString,QVariant> ValueMap=_fromYumFormat(sendString,nextLevel);
            if(!isReturn)
                operator [](NameValue)=ValueMap;
            else
                ReturnMap[NameValue]=ValueMap;
            i+=unusedsize;
        }
    }
    return ReturnMap;
}
void RecursionArray::operator<<(AbstractStruct* h)
{
    this->operator [](h->GetName())=h->GetAllValues();
}

QMap<QString,QVariant> RecursionArray::_fromCfgFormat(QString yum, bool isReturn)
{
    QStringList fromBR=yum.split("\n");
    QMap<QString,QVariant> ReturnMap;
    for(int i=0;i<fromBR.size();i++)
    {
        QString ValueString=fromBR.value(i);
        ValueString=ValueString.trimmed();
        if(ValueString[0]=='#') continue;
        int position=ValueString.indexOf("=");
        if(position<=0 && ValueString.indexOf("{")<=0 && ValueString.indexOf("}")<=0) continue;
        QString NameValue=ValueString.mid(0,position);
        QString tmp=ValueString.mid(position+1);
        QStringList ListValued;
        ListValued << NameValue.mid(0,1);
        ListValued << NameValue.mid(2);
        ListValued << ValueString.mid(position+1);
        NameValue=DeleteQuotes(ListValued.value(1));
        QVariant Value;
        if(ListValued.value(0)=="I") Value=ListValued.value(2).toInt();
        if(ListValued.value(0)=="D") Value=ListValued.value(2).toDouble();
        else if(ListValued.value(0)=="S") {if(!tmp.isEmpty()) Value=ListValued.value(2);}
        else if(ListValued.value(0)=="B")
        {
            if(ListValued.value(2).toLower().indexOf("false")>=0) Value=false;
            else if(ListValued.value(2).toLower().indexOf("true")>=0) Value=true;
        }
        if(ValueString.indexOf("{")<=0)
        {
            if(!isReturn)
                operator [](NameValue)=Value;
            else
                ReturnMap[NameValue]=Value;
        }
        else
        {

            int unusedsize=0,balance=0;bool stop=true; QString sendString;
            while (stop) {
                QString x=fromBR.value(i+unusedsize+1);
                if(unusedsize>fromBR.size()-i) stop=false;
                if(x.indexOf("{")>=0)
                {
                    balance++;unusedsize++;sendString+=fromBR.value(i+unusedsize)+"\n";}
                else if(x.indexOf("}")>=0)
                {
                    if(balance>0) {
                        balance--;unusedsize++;sendString+=fromBR.value(i+unusedsize)+"\n";}
                    else {
                        stop=false;
                    }
                }
                else
                {
                    unusedsize++;
                    sendString+=fromBR.value(i+unusedsize)+"\n";
                }
            }
            QMap<QString,QVariant> ValueMap=_fromCfgFormat(sendString);
            QString sNameValue=DeleteQuotes(ValueString.remove("{").split(" ").value(0));
            if(!isReturn)
                operator [](sNameValue)=ValueMap;
            else
                ReturnMap[sNameValue]=ValueMap;
            i+=unusedsize;
        }
    }
    return ReturnMap;
}
RecursionArray::RecursionArray(QMap<QString,QVariant> h)
{
    QMap<QString,QVariant>::operator =(h);
}
QMap<QString,QVariant> RecursionArray::_fromHTMLTegsFormat(const QString value, bool isReturn)
{
    QMap<QString,QVariant> ReturnValue; //Возвращаемый массив
    int i=0;
    QString NameValue;
    QString sValue;
    while(i<value.size()) //Главный цикл
    {
        int iMin=value.indexOf("<",i); //Поиск символа <
        if(iMin<0) break;
        int iMax=value.indexOf(">",iMin); //Поиск символа >
        if(iMax<0) break;
        //Имя переменной
        NameValue=value.mid(iMin+1,iMax-iMin-1);
        int nMax=value.indexOf("</"+NameValue+">",iMax); //Поиск закрывающего тега
        if(nMax==-1) //Если nMax невалидный
        {
            i=iMax+(iMax-iMin);
            continue;
        }
        sValue=value.mid(iMax+1,nMax-iMax-1);
        int nextiMin=sValue.indexOf("<"+NameValue+">"); //Поиск <ИмяПеременной>
        if(nextiMin!=-1){//Если существует еще по крайней мере один <ИмяПеременной>
            int numNextiMin=0; //Колличество включений <ИмяПеременной>
            while(nextiMin!=-1 && nextiMin<nMax) //Получаю колличество включений
            {nextiMin=sValue.indexOf("<"+NameValue+">",nextiMin+(iMax-iMin));
                numNextiMin+=1; }
            nextiMin=nextiMin+iMax;
            while(numNextiMin!=0) //Использую колличество включений для определения последнего закрывающего тега
            {nMax=value.indexOf("</"+NameValue+">",nMax+(iMax-iMin));
                numNextiMin-=1;}
            sValue=value.mid(iMax+1,nMax-iMax-1);
        }
        if(nMax<0) //Если nMax невалидный
        {
            i=iMax+2;
        }
        QMap<QString,QVariant> temp=_fromHTMLTegsFormat(sValue,true); //Рекурсия
        if(sValue.isEmpty() || NameValue.isEmpty()) {
            int tmp=iMax-iMin;
            i=nMax+tmp;
            continue; }
        if(temp.isEmpty()) //Если рекурсия не нашла других переменных в переменной
        {
            if(!isReturn)
                operator [](NameValue)=sValue;
            else
                ReturnValue[NameValue]=sValue;
        }
        else
        {
            if(!isReturn)
                operator [](NameValue)=temp;
            else
                ReturnValue[NameValue]=temp;
        }
        int tmp=iMax-iMin; //Переход к следующему
        i=nMax+tmp;
    }
    return ReturnValue; //Возврат готового Map
}
/*QMap<QString,QVariant> RecursionArray::_fromArcanFromat(const QString value)
{
    if(value.isEmpty()) return RecursionArray();
    QMap<QString,QVariant> ReturnValue; //Возвращаемый массив
    int i=0;

    while(i<value.size()) //Главный цикл
    {
        QString NameValue;
        QString sValue;
        int iMin,iMax;
        bool isNameReplace=false,isValueReplace=false;
        iMin=value.indexOf("[",i); //Поиск символа [
        if(iMin<0) break;
        if(iMin>0)
        if(value[iMin-1]=='\\')
        {
            while(value[iMin-1]=='\\'){
                iMin=value.indexOf("[",iMin+1); //Поиск символа [
                if(iMin<0) break;
                isNameReplace=true;
            }
        }
        iMax=value.indexOf("]",iMin); //Поиск символа ]
        if(iMax<0) break;
        if(value[iMax-1]=='\\')
        {
            while(value[iMax-1]=='\\'){
                iMax=value.indexOf("]",iMax+1); //Поиск символа ]
                if(iMax<0) break;
                isValueReplace=true;
            }
        }
        NameValue=value.mid(i,iMin-i);
        if(isNameReplace)
            NameValue.replace("\\[","[").replace("\\]","]");
        sValue=value.mid(iMin+1,iMax-iMin-1);
        if(isValueReplace)
            sValue.replace("\\[","[").replace("\\]","]");
        int nextiMin=sValue.indexOf("["); //Поиск [
        if(nextiMin!=-1){//Если существует еще по крайней мере один [
            int numNextiMin=0; //Колличество включений
            while(nextiMin!=-1 && nextiMin<iMax) //Получаю колличество включений
            {nextiMin=sValue.indexOf("[",nextiMin+1);
                numNextiMin+=1; }
            nextiMin=nextiMin+iMax;
            while(numNextiMin!=-1) //Использую колличество включений для определения последнего закрывающего тега
            {iMax=value.indexOf("]",iMax+1);
                numNextiMin-=1;}
            sValue=value.mid(iMin+1,iMax-iMin);
            QMap<QString,QVariant> temp=_fromArcanFromat(sValue); //Рекурсия
            ReturnValue[NameValue]=temp;
            i=iMax+2;
            continue;
        }
        ReturnValue[NameValue]=sValue;
        i=iMax+1;
    }
    return ReturnValue; //Возврат готового Map
}*/
QMap<QString,QVariant> RecursionArray::_fromArcanFromat(const QByteArray s){
        int i = 0;
        int max = s.size();
        QMap<QString,QVariant> result;
        QByteArray name; //ДИНАМИЧНЫЙ БУФЕР НА 10(БАЙТОВ СТАРТОВЫЙ НАБОР)
        QByteArray buffer; //ДИНАМИЧНЫЙ БУФЕР НА ХРАНЕНИЕ БАЙТОВ
        while(i!=max){
            int c = 0; //[ КОЛИЧЕСТВО [ + 1, ] -1 ЕСЛИ 0 ЗНАЧИТ ЗАКРЫТИЕ МАССИВА
            bool typeIS = false; // ПРИСУТСТВИЕ ТИПИЗАЦИИ
            bool isNameNumber = true; // ТЕКУЩЕЕ ИМЯ ИМЕЕТ ОДНИ ЛИШЬ ЦИФРЫ
            char typeTip = 0; //ТИП ТИПИЗАЦИИ, 1 БАЙТ
            while(i!=max){ // сборка буфера имени, "text["
                char a = s[i];
                i = i + 1;

                if(a==64){ // СИМВОЛ @
                    char ta = s[i]; //СЛУДУЮЩИЙ БАЙТ
                    if(ta!=91){ //СЛУДУЮЩИЙ БАЙТ НЕ РАВЕН ОТКРЫТОМУ МАССИВУ [
                        typeTip = ta; //ЗАПИСАТЬ
                        typeIS = true;
                        i = i + 1;
                        continue;
                    }
                }else

                if(a==92 && i!=max){
                    char ta = s[i]; //СЛУДУЮЩИЙ БАЙТ
                    if(ta==91 || ta==93 || ta==92 || ta==64){ //ЕСЛИ СЛЕДУЮЩИЙ БАЙТ ТРЕБУЕТ ЭКРАНИЗАЦИИ, ЭКРАНИРОВАТЬ ЕГО
                        name.append(ta);
                        i = i + 1;
                        continue;
                    }
                }else

                if(a == 91){ // [ МАССИВ ОТКРЫТ
                    c = c + 1;
                    if(c==1)break;
                }
                if(isNameNumber){
                    if(a>57 || a<48){
                        isNameNumber = false;
                    }
                }
                name.append(a); //ЗАПИСЬ БАЙТА В БУФЕР
            }
            if(c==0){ //ошибка символ [ не был найден
                return result;
            }
            //BUFFER
            bool dopRascoding = false; //BOOLEAN УКАЗАЮЩИЙ ЧТО ЭТО МАССИВ
            bool replacer = false; // INT \[
            while(i!=max){
                char a = s[i];
                i = i + 1;

                if(a==92){
                    char ta = s[i]; //СЛУДУЮЩИЙ БАЙТ
                    if(ta == 91 || ta==92 || ta==93){ //ЕСЛИ СЛЕДУЮЩИЙ БАЙТ ТРЕБУЕТ ЭКРАНИЗАЦИИ ПОМЕТИТЬ КАК ТРЕБУЕТ
                        buffer.append(92);
                        buffer.append(ta);
                        replacer = true;
                        i = i + 1;
                        continue;
                    }
                }else


                if(a==91){ //[
                    c = c + 1;
                    dopRascoding = true; // ЭТО МАССИВ, ПРИНЯТЬ МЕРЫ
                }else

                if(a==93){ //] СИМВОЛ ЗАКРЫТИЯ МАСИВА
                    c = c - 1;
                    if(c == 0){ // закрыто
                        if(dopRascoding){ //ЭТО МАССИВ
                            result[QString::fromUtf8(name)]=_fromArcanFromat(buffer);
                        }else {
                            if(replacer){
                                QByteArray buffByte = buffer;
                                buffer.clear();
                                int maxB = buffByte.size();
                                QByteArray replacerBuff;
                                int y = 0;
                                while(y!=maxB){
                                    char aB = buffByte[y];
                                    y = y + 1;

                                    if(aB==92 && y!=maxB){ //поддержка экрана
                                        char ta = s[i]; //СЛУДУЮЩИЙ БАЙТ
                                        if(ta==91 || ta==93 || ta==92){
                                            replacerBuff.append(ta);
                                            y = y + 1;
                                            continue;
                                        }
                                    }
                                    replacerBuff.append(aB);
                                }
                                buffer.clear();
                                buffer.append(replacerBuff);
                                replacerBuff.clear();
                            }

                            if(typeIS==false){
                                result[QString::fromUtf8(name)]=QString::fromUtf8(buffer);
                            }else {

                                if(typeTip==49){
                                    if(buffer.size() == 1){ // ЕСЛИ ЗНАЧЕНИЕ ИМЕЕТ РАЗМЕР В 1 И ВНУТРИ ЗНАЧЕНИЕ 1 ТО ЭТО TRUE
                                        result[QString::fromUtf8(name)]=true;
                                    }else {
                                        result[QString::fromUtf8(name)]=false;
                                    }
                                }else

                                if(typeTip==73){
                                    result[QString::fromUtf8(name)]=QString::fromUtf8(buffer).toInt();
                                }else
                                    if(typeTip==84){
                                        if(buffer.size()>4)
                                            result[QString::fromUtf8(name)]=0;
                                        else
                                        {
                                            unsigned int resu=0;
                                            if(buffer.size()>0) resu=resu+((unsigned char)(buffer.at(0)));
                                            if(buffer.size()>1) resu=resu+((unsigned char)(buffer.at(1)))*256;
                                            if(buffer.size()>2) resu=resu+((unsigned char)(buffer.at(2)))*256*256;
                                            if(buffer.size()>3) resu=resu+((unsigned char)(buffer.at(3)))*256*256*256;
                                            result[QString::fromUtf8(name)]=*((int*)(&resu));
                                        }
                                    }
                                else

                                if(typeTip==68){
                                    result[QString::fromUtf8(name)]=QString::fromUtf8(buffer).toDouble();
                                }else

                                if(typeTip==76){
                                    result[QString::fromUtf8(name)]=QString::fromUtf8(buffer).toInt();
                                }else {
                                    result[QString::fromUtf8(name)]=false;
                                }
                            }
                        }
                        break;
                    }

                }

                buffer.append(a);
            }
            name.clear();
            buffer.clear();
        }
        return result;
    }
QString RecursionArray::toYum()
{
    return _toYUMFormat(*this);
}
QString RecursionArray::toArcan()
{
    return _toArcanFormat(*this);
}
QByteArray RecursionArray::toCfg()
{
    return _toCFGFormat(*this);
}
QString RecursionArray::print()
{
    return printMap(*this);
}
