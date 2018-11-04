#ifndef DECODER_H
#define DECODER_H

#include <QObject>

class Decoder : public QObject
{
    Q_OBJECT
protected:
    Decoder(){}
    Decoder(QByteArray &byte_string){String_to_decode = byte_string;}
    QByteArray String_to_decode;
public:
    static Decoder* get_decoder(QByteArray &byte_string);
    virtual ~Decoder(){}
    virtual bool decode() = 0;
signals:
    void send_string(QString);
};

class Corshyn_decoder:public Decoder
{
private:
    bool decode();
public:
    Corshyn_decoder(QByteArray &byte_string):Decoder(byte_string){}
};

class Drofa_decoder:public Decoder
{
private:
    bool decode();
public:
    Drofa_decoder(QByteArray &byte_string):Decoder(byte_string){}
};

class Error_decoder:public Decoder
{
private:
    bool decode();
public:
    Error_decoder(){}
};

#endif // DECODER_H
