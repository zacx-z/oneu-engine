require 'AuxOUE'
class TitleScene < OUE::Scene
	def initialize
		super
		@title = AuxOUE::Actor.new("title.png");
		@title.ox = 0.5
		@title.x = 400
		@title.y = 100
		self.RS.addChild(@title)

		@menu = []

		sp = AuxOUE::Actor.new('menu\start_game.png')
		self.RS.addChild(sp)

		sp.ox = 0.5
		sp.x = 400
		sp.y = 450

		@menu << sp

		sp = AuxOUE::Actor.new('menu\end_game.png')
		self.RS.addChild(sp)
		
		sp.ox = 0.5
		sp.x = 400
		sp.y = 500

		@menu << sp
		@index = 0

		@menu[0].status = AuxOUE::Actor::FLASH

		@inputcontroller = AuxOUE::InputReceiver.new
		@inputcontroller.key{|event|
			if event.isPress then next true end
			if event.getCode == OUE::IControl::IK_DOWN then
				@menu[@index].status = AuxOUE::Actor::NORMAL
				@index = (@index + 1) % 2
				@menu[@index].status = AuxOUE::Actor::FLASH
			end
			if event.getCode == OUE::IControl::IK_UP then
				@menu[@index].status = AuxOUE::Actor::NORMAL
				@index = (@index + 2 - 1) % 2
				@menu[@index].status = AuxOUE::Actor::FLASH
			end
			if event.getCode == OUE::IControl::IK_RETURN then
				if @index == 0 then $Game.replaceScene(MainScene.new) end
				if @index == 1 then $Game.quit end
			end
			next true
		}
		self.pushInputFocus(@inputcontroller)
	end
	def update
		@menu.each{|x| x.update}
	end
end

class MainScene < OUE::Scene
	def initialize
		super
		@plane = AuxOUE::Actor.new('plane_list.png')
		self.RS.addChild @plane
		@plane.srcRect = OUE::Rectf_t.new(0.0, 0.0, 0.25, 1.0)
		@plane.setFrame(4, 1)
		@plane.frameLen = 10
		@plane.play

		@inputcontroller = AuxOUE::InputReceiver.new
		@inputcontroller.key{|event|
			if event.isPress then next true end
			code = event.getCode
			if code == OUE::IControl::IK_ESCAPE then
				if @pause then self.resume
				else self.pause end
			end
			next true
		}
		self.pushInputFocus @inputcontroller
	end
	def update
		if @pause then return end
		if $Control.keyIsDown(OUE::IControl::IK_LEFT) then
			@plane.x -= 3
		end
		if $Control.keyIsDown(OUE::IControl::IK_RIGHT) then
			@plane.x += 3
		end
		if $Control.keyIsDown(OUE::IControl::IK_UP) then
			@plane.y -= 3
		end
		if $Control.keyIsDown(OUE::IControl::IK_DOWN) then
			@plane.y += 3
		end
		@plane.update
	end
	def pause
		@pause = true
	end
	def resume
		@pause = false
	end
end
$Game = OUE.GetGame()
$Game.init("Star Fighter", 800, 600, true)
#initialize global variables
$Control = OUE.GetControl
$Game.replaceScene(TitleScene.new)
$Game.run()
