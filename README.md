read this document: in english, на русском, українською мовою

# Untranslable

This program is developed to make machine translation difficult. For this purpose, it replaces symbols in the raw text to the same looking symbols from the other alphabet.

### Prehistory

During furious freelancing for the EU company I had to deal with devices, which originally had English manual. In order to apply it to the local production line, they used fast and free machine translation. Tranlation mistakes led to various incidents in factory which resulted to breakdowns and even near miss. 

One of the ways to force company to hire human translator is to make machine translation difficult and pay attention to important notes inside this documentation.


### Getting started

First, you need to download project files

```
git clone https://github.com/xdevelnet/untranslable.git
```

Enter project directory

```
cd untranslable
```

Build project

```
make
```

Program is ready! Now you can run it, passing as an argument valid UTF-8 or ASCII file.
Result will be stored in file with same name, but with "u." prefix. Let's test it on the built-in example!

```
./untranslable examples/example_en.txt
```

Now result file `u.example_en.txt` is stored in `examples` directory.


### Features

Theoretically the program can handle search&replace ANY kind of UTF-8 glyph. Currently latin->cyrillic and cyrillic->latin support exists in database.

Build&run requirements:

1. Any POSIX-compatiable OS
2. C99 compiler


# Untranslable


Эта программа предназначена для того, чтобы усложнить машинный перевод текстов с одного языка на другой. Для этого происходит замена символов в исходном тексте на символы из другого алфавита или языка, но одинаковые по виду.

### Предыстория

Во время неистового рабочего процесса на фрилансе на одну европейскую компанию приходилось иметь дело с утройствами, инструкция к которым была изначально написана на английском языке. Для того чтобы адаптировать её под местое производство, был использован быстрый и бесплатный машинный перевод. Из-за неточностей, которые возникли в следствии такого перевода, были различные инциденты на предприятии, которые приводили к поломкам и чуть не привели к человеческим травмам.

Один из способов заставить компанию потратиться на человеческого переводчика — это максимально усложнить машинный перевод, чтобы всё-таки обратить внимание на важные замечания внутри этой самой документации.

### Начало работы

Сначала вам необходимо скачать файлы проекта

```
git clone https://github.com/xdevelnet/untranslable.git
```

Зайдите в каталог проекта

```
cd untranslable
```

Соберите проект

```
make
```

Программа готова! Теперь вы можете её запустить, передав в качестве аргумента валидный файл в кодировке UTF-8 или ASCII. Результат работы будет в файле с таким же названием как и оригинальный, но с префиксом "u.". Давайте проверим на примере, который имеется в комплекте с проектом!
```
./untranslable examples/example_ru.txt
```
В каталоге `examples` имеется файл `u.example_ru.txt` с результатом работы.

### Особенности

Теоретически программа может поддерживать поиск и замену ЛЮБЫХ глифов UTF-8.
На текущий момент в базе данных имеется замена латиницы на кириллицу и кириллицы на латиницу.

Требования для успешной сборки и работы:

1. Любая современная POSIX-совместимая ОС
2. C99 компилятор
