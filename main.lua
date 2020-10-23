t.init("test1",500,500)
k = 0
tt = 0

while(1)
do
	if(t.getEvt()==256) then
		break
	else
		k = t.getKey()
		if(k~=-1) then --string.byte("q")
			t.p(k.."\n")
		end
		m = math.mod(tt,2)
		t.p(m.."\n")
		if(m==0) then
			t.img("sprite.png",0,0,100,100,100,300,0,0)
		else
			t.img("sprite.png",100,100,100,100,100,300,0,0)
		end
		t.d(100)
	end
	tt=tt+1
end
t.quit()