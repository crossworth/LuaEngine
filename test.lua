-- x = io.read()
-- print(x)


print("")
print("")

player = {
	name = "Meu player",
	sprite =  {
		running = {
			frameCounter = 6,
			frame1 = { time = 100, texture = "res/sprite1.png"},
			frame2 = { time = 200, texture = "res/sprite2.png"},
			frame3 = { time = 300, texture = "res/sprite3.png"},
			frame4 = { time = 400, texture = "res/sprite4.png"},
			frame5 = { time = 500, texture = "res/sprite5.png"},
			frame6 = { time = 600, texture = "res/sprite6.png"},
		}
	},
	sprite2 =  {
		running = {
			frameCounter = 6,
			times = {
				100,
				200,
				300,
				400,
				500,
				600,
			},
			frames = {
				"res/sprite1.png",
				"res/sprite2.png",
				"res/sprite3.png",
				"res/sprite4.png",
				"res/sprite5.png",
				"res/sprite6.png",
			},

		}
	},

	hp = 100,
}
