#SingleInstance, Force
started:=false
Location:=false
Global escaped:=false
Global scrollCount:=0
^!+s:: ;Start mousetrap
if(!escaped){
		centerRadius:= 175  ;175 for slither.io  and 350 for agar.io
		CircleClip(centerRadius)
		MouseGetPos, x, y
			centerX:=x
			centerY:=y
		started=true
		Global escaped=true
	}
Return
^!c::
	Global scrollCount
	if(scrollCount<=15){
		centerRadius+=50
		scrollCount++
	}
return
^!z::
	Global scrollCount
	if(scrollCount>=-2){
		centerRadius-=50
		scrollCount--	
	}
return

Esc::  ;get out of mousetrap
if(escaped){
	CircleClip(0)
	Global escaped=false	
}
return
^!s:: ;Start mousetrap
if(!escaped){
		if(started){
			MouseMove,centerX,centerY,0
			Location=true
			CircleClip(centerRadius)
			Global escaped=true
		}
	}
Return

CircleClip(radius=0, x:="", y:=""){
	global CircleClipRadius, CircleClipX, CircleClipY
	static hHookMouse, _:={base:{__Delete: "CircleClip"}}
	If (radius>0)
		if(!location){
		CircleClipRadius:=radius
		, CircleClipX:=x
		, CircleClipY:=y
		, hHookMouse := DllCall("SetWindowsHookEx", "int", 14, "Uint", RegisterCallback("CircleClip_WH_MOUSE_LL", "Fast"), "Uint", 0, "Uint", 0)
		}
		else{
		Location=false
		CircleClipRadius:=centerRadius
		, CircleClipX:=centerX
		, CircleClipY:=centerY
		, hHookMouse := DllCall("SetWindowsHookEx", "int", 14, "Uint", RegisterCallback("CircleClip_WH_MOUSE_LL", "Fast"), "Uint", 0, "Uint", 0)
		
		}
	Else If (!radius && hHookMouse){
		DllCall("UnhookWindowsHookEx", "Uint", hHookMouse)
		CircleClipX:=CircleClipY:=""
	}
}

CircleClip_WH_MOUSE_LL(nCode, wParam, lParam){
	global CircleClipRadius, CircleClipX, CircleClipY
	Critical
	if !nCode && (wParam = 0x200){ ; WM_MOUSEMOVE 
		nx := NumGet(lParam+0, 0, "Int") ; x-coord
		ny := NumGet(lParam+0, 4, "Int") ; y-coord
		
		If (CircleClipX="" || CircleClipY="")
			CircleClipX:=nx, CircleClipY:=ny
		  
		dx := nx - CircleClipX
		dy := ny - CircleClipY
		dist := sqrt( (dx ** 2) + (dy ** 2) )

		if ( dist > CircleClipRadius ) {
			dist := CircleClipRadius / dist
			dx *= dist
			dy *= dist
			
			nx := CircleClipX + dx
			ny := CircleClipY + dy
		}
		else if(dist <(CircleClipRadius-1)){  ;CircleClipRadius-100
			dist := (CircleClipRadius-1) / dist
			dx *= dist
			dy *= dist
			nx := CircleClipX + dx
			ny := CircleClipY + dy		
		}

		DllCall("SetCursorPos", "Int", nx, "Int", ny)
		Return 1
		
	}else Return DllCall("CallNextHookEx", "Uint", 0, "int", nCode, "Uint", wParam, "Uint", lParam) 
} 

 ^Esc::ExitApp