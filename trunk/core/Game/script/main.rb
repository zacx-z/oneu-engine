require 'OUE'

class MySprite < OUE::Sprite
	def initialize(file)
		super(file)
		@dx = rand(2) - 1
		@dy = rand(2) - 1
		self.x= rand(800)
		self.y= rand(600)
		self.scaleX = -1
		self.srcRect = OUE::Rectf_t.new(1, 1, 0, 0)
	end
	def update
		#self.x += @dx
		#self.y += @dy
	end
end

class MainScene < OUE::Scene
	def initialize
		super
		@sp = MySprite.new("../picture.png")
		self.RS.addChild(@sp)

		@label = OUE::Label.new(200, 600, 20);
		self.RS.addChild(@label, 10);

		@list = [@sp]
	end
	def update dt
		@label.text= "FPS:#{$Game.getFPS()}\nSprite Nums:#{@list.size}\n"

		@list.each{|x| x.update}

		if $Game.getFPS() > 30 then
			sp = MySprite.new("../picture.png")
			self.RS.addChild(sp, 0)

			@list << sp;
		end
	end
end
def NewScene(scene)
	$Scene = scene
	$Game.replaceScene($Scene)
end
srand
$Game = OUE.GetGame()
OUE::Aux_GameInit("This is a game", 800, 600, true)
$Video = OUE::GetVideo();
$Video.getAvailableMode().each do |x|
	prompt "#{x.size.x}, #{x.size.y}, #{x.refreshRate}"
end
NewScene(MainScene.new)

$Game.run()
