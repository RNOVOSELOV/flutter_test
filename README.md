# 🎋 Source File Analyzer
Консольное приложение, с помощью которого можно быстро проанализировать зависимости в исходных файлах. 
## О проекте

ПО написано с применением архитектурного паттерна Model-View-Presenter. И легко адаптируется под любой язык программирования.
UML диаграмма классов представлена ниже, либо [открыть в большом размере (SVG)](https://raw.githubusercontent.com/RNOVOSELOV/sources_analyzer/main/images/UML.svg).

<p align="center">
  <img src="https://github.com/RNOVOSELOV/sources_analyzer/blob/main/images/UML.svg"/>
</p>

Результат работы программы - дерево зависимостей файлов, а также частота включений. 
На текущий момент полностью реализована поддержка следующих языков программирования:
- С++ (".h" ".hpp" ".cpp");
- Dart (".dart").

## Работа ПО
Анализатор - консольное приложение. Для запуска необходимо выполнить команду:
> analyser \<PATH\> [опции]

В качестве PATH указывается путь до анализируемого файла либо директории. 
Если PATH - директория, то дерево зависимостей строится для всех файлов внутри каталога и его поддиректорий.

Возможные опции:

-	-C анализируются исходные файлы языка С++ (используется по умолчанию)
-	-D анализируются исходные файлы языка Dart (адаптировано для Flutter)
-	-I путь для поиска библиотечных заголовков
-	-P путь до корневой директории проекта (необходима для некоторых языков программирования)


Пример вызова программы:

> analyser c:\mysources\ -I c:\mysources\includes -I c:\mylibrary -С

> analyser -I c:\mysources\includes -D c:\mysources\main.cpp -I c:\mylibrary

> analyser -I c:\mysources\includes -I c:\mylibrary c:\mysources\main.cpp

<p align="center">
  <img src="https://github.com/RNOVOSELOV/sources_analyzer/blob/main/images/result_1.png" height="300"/>
</p>

Если в дереве уже присутствует добавляемый PATH, то при выдаче результата он помечается как REP. Дальнейший проход внутрь файла не осуществляется. 
Благодаря этому уменьшается дерево за счет исключения дублирующих зависимостей, а так же устраняется проблема «циклических» зависимостей. 

Метка DEL ставится у файлов, которые не удалось обнаружить на файловой системе. Для локальных заголовочных файлов в каталоге с файлом, для библиотечных в одном из путей переданных с флагом -I.

## Развитие проекта

### Версия 1.2

- Реализация фабричного метода реализации классов моделей для различных языков программирования.

### Версия 1.1

- Абстракция модели, возможность быстрой адаптации под другие языки программирования;
- Возможность анализировать отдельные файлы.

### Версия 1.0

- Minimal Viable Product - построение дерева зависимостей для всех файлов в каталоге, отображение списка частот включений файлов;
- Абстракция отображения, возможность быстрого "переезда" на "оконный" интерфейс в будущем.

## Индикация ошибок

#### Возможные ошибки:

- Некорректный запуск программы

    > Error! Number of arguments error. Source path isn't setted.
	
- Не указан анализируемый каталог или файл
	
    > Error! Source's path not exist on filesystem.

- Не указана директория проекта
	
    > Error! Need to set progect directory for this language. Use -P parameter.

#### Предупреждения:

- Через флаг -I указана несуществующая на файловой системе директория 

    > WARNING! Directory \<directory path\> doesn’t exist!
  
- В исходном файле обнаружен библиотечный заголовочный файл, отсутствующий в директориях, перечисленных с помощью опции -I. В дереве такой заголовочный файл помечается как DEL.
  
    > WARNING! In \<filename\>: header \<library header\> not found in -I directories!
