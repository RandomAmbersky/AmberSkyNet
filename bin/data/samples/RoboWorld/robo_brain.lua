--print("Hmm... I'm thinking...");

--SetParam("AI_type", "roboot");
--par = GetParam("AI_type");
--print( par );

--rnd = math.random(100);
--if ( rnd > 80 ) then 
--    Rotate(3) 
--else if ( rnd < 20 ) then Rotate(-3) end;
--end;

x = GetX();

y = GetY();

z = GetZ();

rot = GetRot();

sec = GetSector( rot );

--print( r_sec );

if (x>100) then
    if ( sec == 1 ) then Rotate(90); end;
    if ( sec == 2 ) then Rotate(90); end;
    if ( sec == 7 ) then Rotate(-90); end;
    if ( sec == 8 ) then Rotate(-90); end;
    end;

if (y>100) then
    if ( sec == 1 ) then Rotate(-90); end;
    if ( sec == 2 ) then Rotate(-90); end;
    if ( sec == 3 ) then Rotate(90); end;
    if ( sec == 4 ) then Rotate(90); end;
    end;

if (x<0) then
    if ( sec == 3 ) then Rotate(-90); end;
    if ( sec == 4 ) then Rotate(-90); end;
    if ( sec == 5 ) then Rotate(90); end;
    if ( sec == 6 ) then Rotate(90); end;
    end;
    
if (y<0) then
    if ( sec == 5 ) then Rotate(-90); end;
    if ( sec == 6 ) then Rotate(-90); end;
    if ( sec == 7 ) then Rotate(90); end;
    if ( sec == 8 ) then Rotate(90); end;
    end;
    
MoveForward();
