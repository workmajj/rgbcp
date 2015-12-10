rgbcp
=====

Copy data between MacBooks visually, using flashing screens and cameras

**Warning!** The transmit portion of this project **rapidly flashes the entire screen** (at ~15 Hz) in order to send data. Please be forewarned in case you or others are sensitive to flashing lights. You can still read the source, though!

Contact
-------

John J. Workman ([@workmajj](https://twitter.com/workmajj))

I worked on this software during my time as a [Recurse Center](https://www.recurse.com/) facilitator. If you'd like to join a warm community of programmers dedicated to self-improvement, you should consider applying. :-)

(Thanks to [Jess Sorrell](https://twitter.com/OptimistsInc) and [Tom Ballinger](https://twitter.com/ballingt) for [inspiring this hack](#description)!)

Description
-----------

This project consists of two programs which together let you copy data between MacBooks visually, without traditional direct or networked connections. The programs&mdash;a transmitting half (`tx`) and a receiving half (`rx`)&mdash;operate after aligning the two computers physically, so the transmitting laptop's screen faces the receiving laptop's camera. The transmitting MacBook then flashes its screen in a pattern representing binary data, which the receiving machine sees and decodes.

Because of how `rgbcp` accesses the camera&mdash;piping in raw bytes via `ffmpeg`'s `avfoundation` device&mdash;it does require OS X. The C source is platform-independent, however. At some point I may write camera code in Objective-C.

I was inspired to do this project after reading [a tweet](https://twitter.com/OptimistsInc/status/642854433458626561) in which [Jess Sorrell](https://twitter.com/OptimistsInc) and [Tom Ballinger](https://twitter.com/ballingt) described similar work. I didn't know the details of their implementation, though, so I started imagining how I would solve the problem from scratch. I compared notes with Tom a few weeks later at RC, and it turns out we had very similar approaches. I think Jess and Tom ended up doing a few nice features (e.g., adaptive frames) that hadn't even occurred to me, however!

Theory
------

TODO

Build & Usage
-------------

To use `rgbcp` to copy data between two MacBooks:

1. `rgbcp` depends on `ncurses` for sending and `ffmpeg` for receiving. Recent versions of OS X include `ncurses` by default, but you'll need to install `ffmpeg`. Using Homebrew:

       $ brew install ffmpeg

2. Next, clone the project from GitHub on both computers and build it from source:

        $ git clone https://github.com/workmajj/rgbcp.git
        $ cd rgbcp
        $ make

3. If the transmitting computer's terminal uses a non-standard color palette like Solarized, you'll need to revert it temporarily. Colors don't need to be exact, but red, green, and blue should look roughly correct.

4. Now pick some data you'd like to send, and cue up the transmitting computer. Try not to choose something huge because `rgbcp`'s throughput is low (7.5 bytes/sec)&mdash;text will do just fine. Type this, but don't run it yet:

        $ echo "How funny you are today New York" | ./tx

5. And cue up (but don't run) this on the receiving MacBook:

        ffmpeg -loglevel panic -f avfoundation -pixel_format 0rgb -framerate 30 \
                -video_size 320x240 -i "default:none" -f rawvideo - | ./rx

6. Place both laptops so that they face each other on a level surface, ideally close enough that they touch.

7. Finally, start the receiving computer (it should know to wait on a special input pattern), and then start the transmitting computer. The receiving MacBook will print the text to `stdout` as it comes across and is decoded.

8. Feel free to try this again using binary data. You can pipe in any type of file through `stdin`.

Testing
-------

If you don't have two MacBooks, you can test the receiving half (`rx`) by transmitting data from a mobile device:

1. Starting from the project directory, use your laptop to serve the HTML/JS test pages:

        $ cd test
        $ python -m SimpleHTTPServer 8000

2. Then, in another terminal window, get your machine ready to receive data as usual.

3. Finally, open a test page on your phone by navigating to `http://<laptop_ip_address>:8000/text.html`, and quickly place your phone's display in front of your MacBook's iSight camera (you can basically press the phone to it).

4. If you need to restart the transmission, simply reload the test page.

License
-------

This software is published under the terms of the BSD 3-clause license.

See the [LICENSE](LICENSE.md) file for more info.
