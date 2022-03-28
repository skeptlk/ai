Для реализации программного агента требуется подключить библиотеку CheckersBase.dll. Выполнив следующие пошаговые действия:

* Создать новый ClassLibrary проект в Microsoft Visual Studio 2013. 
* Добавить в References сборку CheckersBase.dll. 
* Затем необходимо переопределить базовый класс BrainBase который находится в библиотеке CheckersBase.dll и реализовать FindMotion метод. Далее показано как это сделать.

```
#!c#

using CheckersBase.BrainBase;
using CheckersRules;

namespace BrainExample
{
  [BrainInfo(BrainName = "IvanovBrain", Student = "Ivanov", StudentGroup = "11МОА")]
  public class Brain :BrainBase
  {
    public override Motion FindMotion(Board Board, bool isWhite)
    {
         //Получаем все возможные ходы для текущей доски
         List<Motion> allMotions = Rules.FindValidMotions(board, isWhite).GetAllMotions();
         //TODO: Здесь необходимо реализовать минимаксный алгоритм выбора наилучшего хода
         //TODO: Вернуть содержащий ход экземпляр класса Motion.
         //Пример формирования хода [4,0] в [0,4]
         var motion = new Motion(new Point(4, 0), new Point(0, 4));
      	 return motion;
    }
```

Далее необходимо скомпилировать и положить полученную библиотеку в одну папку с исполняемым файлом Checkers.exe .