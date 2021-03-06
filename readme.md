# 3D Game Engine Template for MacOS and Windows

## What is this?
If you got here from my Handmade Hero Mac OS platform layer videos, you are looking at my most advanced 3D version of that work. In essence, it is a distillation of the work I have been doing for the past three years. 

This is a 3D game engine that compiles and runs natively on both Mac OS and Windows. It loads and renders 3D models (from obj files), placing them in a 3D coordinate grid. I am planning to use it as a basis for making my next game: Mooselutions 3D. Since this contains no game-specific code or content, I figured I can release this project to the public before starting that venture.

I will be blunt about a few things. If you came here looking for "clean" elegant abstractions, you won't find them. Most of this code is kind of messy but does the job. Some features work on some platforms but don't work yet on other platforms. I'm not much of a "plan ahead" sort of developer, and you will definitely see that style coming through in spades. Functions only exist when they are called in more than one place, and there are "messy" if-defs for Windows/MacOS all over the place because I just needed to get the thing up and running.

You won't make the same decisions I do, nor would I expect you to, but I do think this is a pretty good reference point if you want to dip your toes into making your own 3D game engines. It builds, and it works. What more could you ask for?

Furthermore, for me to even say this is *my* work is only telling a small part of the story. I, like you, am a nexus of many different influences. Much of this code comes from Casey Muratori's work, and he was influenced by Chris Hecker, who was influenced by someone else and so on down the line. We don't choose our influences, but we do choose how to develop our talents.

I'm not really sure what I uniquely bring to the table aside from recognizing the market's need to support Mac OS (and other developers' interest in it). I have been actively working on Mac ports for years because that (for a time) was the only development PC I owned. I sort of see myself as a bridge between the iOS development space (which I work in) and the wider game development space. If I can introduce more of the Apple-centric people to what true cross-platform app/game development looks like, it might cause the culture to shift toward something more manageable.

This is what cross-platform development looks like when we aren't using a giant wobbly tower of abstractions like React Native to accomplish it. You get to work in a compiled language, with your own design tools, using your favorite debugger, natively, on the computer of your choosing. You get near-instant compile times and short iteration cycles because there's no bloat. It's not just a pie-in-the-sky vision. This is it. You are looking at a tangible artifact. I made the thing I want, and now I'm sharing some of it with you (for FREE no less).

## How to build it
Each platform has its own folder with a build script. Navigate to the folder in the terminal and run the build script. The build script will output an executable in the build folder for the platform you are currently building from. If you're coming here from the Handmade Community, it is very similar in structure to what you see in Handmade Hero.

## A Note on Renderers
This engine uses two separate rendering backends. On MacOS, it uses Metal as the renderer. On Windows, it uses D3D11. That's just how things have shaken out when I've done my own game prototyping (mostly based on recommendations from friends). The primary reason is the debugger support on both platforms. You can use Xcode as a rendering debugger on the Mac and RenderDoc + Visual Studio on Windows. It just seemed like the most manageable approach to take.

## About the obj model loader
I threw it together over the course of like three days and it is super hacky. I know for a fact that it doesn't load most obj files. It only loads the ones in this project. Because the obj files I am working with don't have a clear character demarcating the end of the file, I just placed an 'e' at the end of my obj files so my parser knows where to find the end of the face data. So if you want to load your obj file, and you don't have an 'e' at the end like me, your thing is gonna crash. Fix this however you want.

I will make this thing more robust as my level of annoyance increases. For now, it more or less works. These are the sorts of tradeoffs you have to make to be productive in software. Starting with a generic solution means you have to support it, and supporting all conceivable use cases is much more effort than just making the thing that works for what you need right now.

## How to Support This Work
I recognize that this project is in a rough state, but I am putting it out there anyway because people have approached me with great interest. I want to give back to the community, and financial support helps to make this work sustainable and justifiable.

If this receives regular paid contributions and publicity, I can dedicate more time to improving it. With more time and effort put in, you get the most polished version of the work and can learn more effectively. If you ask a question or would like an improvement made, and you contribute in some way, I will respond with an update to this project.

Here are a few ways you can do that.

- Go to my [Itch Store](https://tedbendixson.itch.io/). Play one of my games and make a donation.

Even if you can't make a contribution, you are welcome to contact me with your questions and concerns. I understand that not everyone is in a position to give, and part of my mission is to make game development more open to people who can't afford expensive computers to run the latest version of Unity or other popular game engines. 

I believe in a more decentralized computing world (albeit a market-based capitalistic one that rewards the competence of the individual). We need more options, not less, and this is my small way of showing you what that other option looks like.

As always, thanks for your interest. Wishing you the best on your projects


