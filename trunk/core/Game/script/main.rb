require 'OUE'

class MainScene < OUE::Scene
	def initialize
		super()
		@sp = OUE.Sprite("../picture.png")
		self.getRenderScene().addChild(@sp)
	end
	def update
		@sp.setX(@sp.getX() + 10)
		puts 'haha'
	end
end

OUE.Game_build()
$Game = OUE.GetGame()
$Game.init("haha", 800, 600, true)
$Game.replaceScene(MainScene.new)
$Game.run()
