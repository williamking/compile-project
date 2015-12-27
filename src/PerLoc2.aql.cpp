create view Cap as
  extract regex /[A-Z][a-z]*/
    on D.text as Cap
  from Document D;

create view Stt as
  extract regex /Washington|Georgia|Virginia/
    on D.text
    return group 0 as Stt
  from Document D;

create view Loc as
  extract pattern (<C.Cap>) /,/ (<S.Stt>)
  return group 0 as Loc
    and group 1 as Cap
    and group 2 as Stt
  from Cap C, Stt S;

create view Per as
  extract regex /[A-Z][a-z]*/
    on D.text
    return group 0 as Per
  from Document D;

create view PerLoc as
  extract pattern (<P.Per>) <token>{1,2} (<L.Loc>)
  return group 0 as PerLoc
    and group 1 as Per
    and group 2 as Loc
  from Per P, Loc L;

output view Cap;
output view Stt;
output view Loc;
output view Per;
output view PerLoc;
