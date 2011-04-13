require "input"
local _G = _G
setfenv(1, input)
local function ipt(button)
	return key_release(button)
end
_G.get_input_letter = function()
	if(ipt(IK_A)) then return 'a' end
	if(ipt(IK_B)) then return 'b' end
	if(ipt(IK_C)) then return 'c' end
	if(ipt(IK_D)) then return 'd' end
	if(ipt(IK_E)) then return 'e' end
	if(ipt(IK_F)) then return 'f' end
	if(ipt(IK_G)) then return 'g' end
	if(ipt(IK_H)) then return 'h' end
	if(ipt(IK_I)) then return 'i' end
	if(ipt(IK_J)) then return 'j' end
	if(ipt(IK_K)) then return 'k' end
	if(ipt(IK_L)) then return 'l' end
	if(ipt(IK_M)) then return 'm' end
	if(ipt(IK_N)) then return 'n' end
	if(ipt(IK_O)) then return 'o' end
	if(ipt(IK_P)) then return 'p' end
	if(ipt(IK_Q)) then return 'q' end
	if(ipt(IK_R)) then return 'r' end
	if(ipt(IK_S)) then return 's' end
	if(ipt(IK_T)) then return 't' end
	if(ipt(IK_U)) then return 'u' end
	if(ipt(IK_V)) then return 'v' end
	if(ipt(IK_W)) then return 'w' end
	if(ipt(IK_X)) then return 'x' end
	if(ipt(IK_Y)) then return 'y' end
	if(ipt(IK_Z)) then return 'z' end
end
