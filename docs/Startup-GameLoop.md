# Startup

One of the goals for startup is to get it done as fast as possible, to ensure the user isn't left waiting for the main menu to appear.  Given this is a small game, the goal is easy to achieve.  Regardless, a small effort was taken to ensure minimal time is spent between starting the game and when the game screen with the main menu appears.

The startup sequence occurs in this order:

1. Read the configuration
1. Initialize the `SoundPlayer` singleton
1. Initialize the `Content` singleton
1. Make the requests for the content necessary to present the main menu
1. Build the application window
1. Initialize the main menu views
1. Enter the game loop

The one item of interest is getting the main menu content loading started.  This involves loading the fonts, audio tics that occur when navigating between menu items, and the background image.  Requests to load assets made to the `Content` singleton do not block.  The request goes into a queue and a worker thread loads things one at a time, in the order items are placed into the queue.

In order to ensure the game loop isn't started until these requests have completed, an `onComplete` lambda is defined, and passed into the last asset request.  The lambda signals an event when it is invoked.  That same event is waited on at the end of the `loadMenuContent` method, preventing it from returning before the assets are all loaded (or an error in loading occurs).  When the last asset request completes, the `onComplete` lambda is invoked, the event is signaled, resulting in the lock at the end of the `loadMenuContent` method to release and the method completes.

No other game content is loaded during startup.  Assets needed for each level aren't loaded until the level is selected for play.  Once a level is selected for play, requests are made for its assets to be loaded and then the level starts once that completes.  The first time a level is played, the game does block until the assets are loaded.  But, the I/O occurs so quickly, it is imperceptible to the typical game player, and only very slightly noticeable if you are aware of it...the delay is on the order of milliseconds.  An alternative I considered, but decided against, is to start loading all level content once the main menu is up and running.  One main reason I decided to not do this; it wasn't necessary.  The time it takes to load the unique assets for each level is tiny, an actual non-issue.

# Game Loop
