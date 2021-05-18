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

The code follows the standard game loop, which looks like...

1. Process input
1. Update the game simulation
1. Render the game state
1. If user selects to quit go to Step 5, else go to Step 1
1. Shutdown

During the _process input_ stage (Step 1) all pending events from the SFML are processed.  Any of the inputs that are input related (keyboard or mouse) are signaled to the currently active `View`.  When a view receives an input signal, it immediately acts upon the input.  However, in the case of the `Gameplay` view the input is passed to the `GameModel`, where it is added to an input queue and then processed when the `update` method is called.  This ensures the game model controls when the inputs are processed.

During the _update the game simulation_ stage (Step 2), the `update` method of the currently active view is called.  This allows the active view to update its state.  The `update` method of all views return the next view that should be displayed.  Most of the time it is simply the active view that is the next view, but not always.  If the user is in the main menu and navigates to the About option, the `MainMenu` view returns `ViewState::About` as the next view (state) to display.  Similarly, when in the `About` view and the user presses the escape key, the `About` view returns `ViewState::MainMenu` as the next view (state) to display.  This is a fairly simple approach to having the code know which view to display, but it works perfectly well for this game, and I didn't see a reason to make it more complex.  The code that checks for and changes the next view to display is shown below...

    if (nextViewState != viewState)
    {
        if (nextViewState == views::ViewState::Exit)
        {
            running = false;
        }
        else
        {
            view->stop();
            view = views[nextViewState];
            view->start();
            viewState = nextViewState;
            if (viewState == views::ViewState::GamePlay)
            {
                window->setMouseCursorVisible(false);
            }
            else
            {
                window->setMouseCursorVisible(true);
            }
        }
    }

During the _render the game state_ stage (Step 3) the `render` method of the currently active view is called.  For the various menu views, the rendering is fairly straightforward, only the `GameModel` object of the `Gameplay` view has any complexity.  The `GameModel` is detailed here (TODO: add a link to the doc for it).

The _shutdown_ stage (Step 5) performs a graceful shutdown of the application.  During this stage the configuration state is persisted, the `SoundPlayer` and `Content` singletons are terminated, along with closing the application window.  Once all of these activities are complete, the process exits.

## Game Loop - Elapsed Time

At each of the stages in the game loop, the elapsed time is passed to the various methods; input signal, `update`, and `render`.  Elapsed time is measured as the last time the elapsed time was computed, which is done at the top of the game loop.  In other projects I have maintained elapsed time in milliseconds, but for this game I decided to maintain elapsed time in microseconds.  While it isn't essential for this game, given the higher frame rates possible on computers/consoles (e.g., 120 Hz), I'm thinking millisecond precision is no longer accurate enough.  The potential loss of fractional milliseconds could build up and have an effect a game in some small way.  Because of this, I decided to make the move to maintaining elapsed time in microseconds.
