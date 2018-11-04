#include "decoder.h"

Decoder* Decoder::get_decoder(QByteArray &byte_string)
{
    Decoder * decod;
    if (byte_string.at(0) == ':')
    {
        decod = new Corshyn_decoder(byte_string);
    }
    else if(quint8(byte_string.at(0)) == 255 && quint8(byte_string.at(1)) == 255)
    {
        decod = new Drofa_decoder(byte_string);
    }
    else
    {
        decod = new Error_decoder();
    }
    return decod;
};

bool Error_decoder::decode()
{
    emit send_string("Ошибка: неверный <НС>\n");
    return false;
}

bool Corshyn_decoder::decode()
{
    emit send_string("ПРМ «Коршун»\n");
    if(String_to_decode.at(String_to_decode.size()-1) != '!')
    {
        emit send_string("Ошибка: неверный <КТ>\n");
        return false;
    }
    else
    {
      for(int i = 1; i < String_to_decode.size()-1; i++)
      {
        QString temp = "";
        if(String_to_decode.at(i) == 'F')
        {      
            temp += String_to_decode.at(i+1);
            temp += String_to_decode.at(i+2);
            temp += String_to_decode.at(i+3);
            temp += String_to_decode.at(i+4);

            int frequency = temp.toInt();
            if(String_to_decode.at(i+4) == '0' && (frequency >= 100 && frequency <= 5000))
            {
                emit send_string("Частота = " + QString::number(frequency) + "\n");
                i+=4;
            }
            else
            {
                emit send_string("Ошибка: неверное значение частоты\n");
                return false;
            }
        }
        else if(String_to_decode.at(i) == 'S')
        {
            if(String_to_decode.at(i+1) == '1')
            {
                emit send_string("Чувствительность = минимальная\n");
            }
            else if(String_to_decode.at(i+1) == '2')
            {
                emit send_string("Чувствительность = средняя\n");
            }
            else if(String_to_decode.at(i+1) == '3')
            {
                emit send_string("Чувствительность = максимальная\n");
            }
            else
            {
                emit send_string("Ошибка: неверное значение чувствительности\n");
                return false;
            }
            i++;
        }
        else if(String_to_decode.at(i) == 'C')
        {
            temp += String_to_decode.at(i+1);
            temp += String_to_decode.at(i+2);

            int channel = temp.toInt();
            if(channel > 0 && channel <= 16)
            {
                emit send_string("Рабочий канал = " + QString::number(channel) + "\n");
                i+=2;
            }
            else
            {
                emit send_string("Ошибка: неверное значение рабочего канала\n");
                return false;
            }
        }
        else
        {
            emit send_string("Ошибка: неверный параметр\n");
            return false;
        }
      }
    }
    return true;
}

bool Drofa_decoder::decode()
{
  emit send_string("ПРД «Дрофа»\n");

  if(quint8(String_to_decode.at(6)) != 239 && quint8(String_to_decode.at(7)) != 239)
  {
      emit send_string("Ошибка: неверный <КТ>\n");
      return false;
  }
  else
  {
    int frequency = quint8(String_to_decode.at(2))*256+quint8(String_to_decode.at(3));
    if (frequency >= 5000 && frequency <= 50000)
    {
       emit send_string("Частота = " + QString::number(frequency) + "Гц\n");
    }
    else
    {
        emit send_string("Ошибка: неверное значение частоты\n");
        return false;
    }

    if (quint8(String_to_decode.at(4)) == 1)
    {
        emit send_string("Режим работы = 1А\n");
    }
    else if (quint8(String_to_decode.at(4)) == 2)
    {
        emit send_string("Режим работы = 2А\n");
    }
    else if (quint8(String_to_decode.at(4)) == 10)
    {
        emit send_string("Режим работы = F5R\n");
    }
    else
    {
        emit send_string("Ошибка: неверное значение режима работы\n");
        return false;
    }

    if (quint8(String_to_decode.at(5)) <= 100 && quint8(String_to_decode.at(5))%10 == 0)
    {
        emit send_string("Мощность = "+ QString::number(quint8(String_to_decode.at(5))) +"%\n");
    }
    else
    {
        emit send_string("Ошибка: неверное значение мощности\n");
        return false;
    }
  }
  return true;
}
