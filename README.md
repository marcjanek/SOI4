Ćwiczenie 4
Synchronizacja procesów z wykorzystaniem monitorów


1. Cel ćwiczenia

Należy napisać program w języku C++ w środowisku systemu Linux realizujący
rozwiązanie problemu zadanego przez prowadzącego ćwiczenie. Problem jest
rozwinięciem ćwiczenia nr 3.


2. Operacje monitorowe

Przykładowa notacja monitora (za Hoare) jest następująca:

type mon = monitor                                 //1
   var ...{zmienne wewnętrzne monitora};
   var c1,...,cP:condition;                        //2
procedure entry X (...);
var ...
begin                                              //3
   if ... then ci.wait;                            //4
   ... {właściwa treść procedury}
   if not cj.empty                                 //5
   then cj.signal                                  //6
end;                                               //7
procedure entry Y ...
...

begin
  {inicjowanie zmiennych wewnętrznych}             //8
end.

Do realizacji monitora służy specjalna biblioteka realizująca następujące
elementy monitora, odwołujące się do wskazanych miejsc w kodzie:

// 1 typ "mon" służący do "powoływania" monitorów, w szczególności
// zawierających zmienną semaforową do wzajemnego wykluczania "mutex". Każdy
// monitor musi zawierać dokładnie jedną zmienną typu "mon": m:mon.

// 2 typ warunków Hoare'a, pozwalający na powoływanie warunków
// synchronizujących.

// 3 wejście do monitora "enter". Musi być umieszczone na początku każdej
// metody monitora i tylko tam: enter(m);

// 4 zawieszenie na warunku: wait(ci);

// 5 badanie niepustości warunku: empty(cj);

// 6 wznawianie procesu: signal(cj);

// 7 wyjście z monitora "leave". Musi być umieszczone na końcu każdej metody
// monitora i tylko tam: leave(m);

// 8 inicjowanie monitora i zmiennych warunkowych: initm(m); initc(c1,m); ...


3. Zadanie do zrealizowania

Należy zrealizować typ "bufor komunikacyjny" przy pomocy monitora. W czasie
implementacji należy zapewnić synchronizację taką samą jak w ćwiczeniu 3.

Należy zrealizować więzy pomiędzy buforami, te same co w ćwiczeniu 3,
zmodyfikowane o dodatkowe warunki wskazane dla ćwiczenia 4 przez prowadzącego.
