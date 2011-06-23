require 'OUE'
module AuxOUE
	class InputReceiver < OUE::InputReceiver
		def initialize()
			super()
		end
		def onMouse(event)
			if @mproc then return @mproc.call event end
			return false
		end
		def onKey(event)
			if @kproc then return @kproc.call event end
			return false
		end
		def onChar(event)
			if @cproc then return @cproc.call event end
			return false
		end
		def mouse(&mproc)
			@mproc = mproc
		end
		def key(&kproc)
			@kproc = kproc
		end
		def char(&cproc)
			@cproc = cproc
		end
	end
	class Actor < OUE::Sprite
		NORMAL = 0
		FLASH = 1
		FLASH_STEP = 5

		attr_reader :status
		attr_accessor :frameLen

		def initialize(filename)
			super(filename)
			@status = NORMAL
			@play = false

			@hframe = 1
			@vframe = 1
			@frameLen = 1
		end
		def setFrame(hframe, vframe)
			@hframe = hframe
			@vframe = vframe
		end
		def play
			@play = true
			@curFrame = 0
			@curFrameQ = 0
			@totalFrame = @hframe * @vframe
		end
		def update
			if @play then
				itl = 1.0 / @hframe
				l = @curFrame * itl
				self.srcRect = OUE::Rectf_t.new(l, 0, l + itl, 1)
				@curFrameQ += 1
				if @curFrameQ > @frameLen then
					@curFrame = (@curFrame + 1) % @totalFrame
					@curFrameQ = 0
				end
			end
			if @status == FLASH then
				if @flash.nil? then
					@flash = 0
					@inc = true
				end
				if @inc then @flash += FLASH_STEP
				else @flash -= FLASH_STEP
				end
				if @flash > 255 then
					@flash = 255
					@inc = false
				end
				if @flash < 0 then
					@flash = 0
					@inc = true
				end

				self.color = [@flash, @flash, @flash]
			end
		end
		def status=(status)
			if @status == FLASH then
				self.color = [0, 0, 0]
			end
			@status = status
		end
	end
end

