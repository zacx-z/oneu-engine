require 'OUE'

class MainScene < OUE::Scene
	def initialize
		super
	end
	def update(dt)
	end
end
def NewScene(scene)
	$Scene = scene
	$Game.replaceScene($Scene)
end
$Game = OUE.GetGame()
$Game.init("This is a game", 800, 600, true)
NewScene(MainScene.new)
$Game.run()
$Game.replaceScene(nil)
