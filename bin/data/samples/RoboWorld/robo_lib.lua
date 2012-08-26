-- определение сектора, в котором располагаетс€ угол поворота в плоскости ’Y против часовой стрелки
--
--                   x
--
--                   ^
--                 \1|8/
--                 2\|/7
--            y <----A----
--                 3/|\6
--                 /4|5\
--                   |
--

function GetSector(z_rot)
    if z_rot<=45 then return 1; end;
    if z_rot<=90 then return 2; end;
    if z_rot<=135 then return 3; end;
    if z_rot<=180 then return 4; end;
    if z_rot<=225 then return 5; end;
    if z_rot<=270 then return 6; end;
    if z_rot<=315 then return 7; end;
    return 8;
end
