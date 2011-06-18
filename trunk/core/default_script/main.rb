require 'OUE'

class MainScene < OUE::Scene
	def initialize
		super
	end
	def update
	end
end
$Game = OUE.GetGame()
$Game.init("This is a game", 800, 600, true)
$Game.replaceScene(MainScene.new)
$Game.run()
