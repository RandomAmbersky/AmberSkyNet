-- common library

--  [ Flip Visible ] --

function FlipVisible(name)
 if GetParam(name,"visible")=='1' then
  SetParam(name,"visible","0")
 else
  SetParam(name,"visible","1")
 end	
end
