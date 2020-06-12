/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';      /* separator between status bars */
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;        /* vertical padding for statusbar */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int showsystray             = 1;   /* 0 means no systray */

static int floatposgrid_x           = 5;        /* float grid columns */
static int floatposgrid_y           = 5;        /* float grid rows */

static const char *fonts[]          = { "monospace:size=10" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"kitty", "--class", "kitty-sp", NULL};

static Sp scratchpads[] = {
	/* name          cmd  */
	{"kitty-sp",     spcmd1},
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

#define RULE(...) { .monitor = -1, __VA_ARGS__ },
#define WTYPE "_NET_WM_WINDOW_TYPE_"
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	RULE(.instance = "kitty-sp", .tags = SPTAG(0), .isfloating = 1, .matchonce = 1, .floatpos = "50% 50% 90% 80%") // Not flagged as isterminal since we don't want the sp to swallow.
	RULE(.type = WTYPE "DIALOG",  .isfloating = 1)
	RULE(.type = WTYPE "UTILITY", .isfloating = 1)
	RULE(.type = WTYPE "TOOLBAR", .isfloating = 1)
	RULE(.type = WTYPE "SPLASH",  .isfloating = 1)
	RULE(.class = "feh",             .tags = 0)
	// RULE(.class = "kitty",           .isterminal = 1)
	RULE(.title = "Discord Updater", .tags = 1 << 4, .isfloating = 1, .matchonce = 1, .floatpos = "50% 50%", .monitor = 1)
	RULE(.class = "discord",         .tags = 1 << 4, .monitor = 1)
	RULE(.class = "lutris",          .isfloating = 1)
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define STATUSBAR "dwmblocks"

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor  bar    alignment         widthfunc              drawfunc              clickfunc           name */
	{ -1,       0,     BAR_ALIGN_LEFT,   width_tags,            draw_tags,            click_tags,         "tags" },
	{ -1,       0,     BAR_ALIGN_LEFT,   width_ltsymbol,        draw_ltsymbol,        click_ltsymbol,     "layout" },
	{ -1,       0,     BAR_ALIGN_RIGHT,  width_status2d,        draw_status2d,        click_statuscmd,    "status2d" },
	{  0 ,      0,     BAR_ALIGN_RIGHT,  width_systray,         draw_systray,         click_systray,      "systray" },
	{ -1,       0,     BAR_ALIGN_NONE,   width_wintitle,        draw_wintitle,        click_wintitle,     "wintitle" },
};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_Tab,   ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_s,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_d,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_g,     ACTION##stack, {.i = -1 } },

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ Mod1Mask,                     XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY,                       XK_backslash,      togglescratch,  {.ui = 0 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	{ MODKEY,                       XK_KP_Home,      floatpos,             {.v = "-26a -26a" } }, // ↖
	{ MODKEY,                       XK_KP_Up,        floatpos,             {.v = "  0a -26a" } }, // ↑
	{ MODKEY,                       XK_KP_Page_Up,   floatpos,             {.v = " 26a -26a" } }, // ↗
	{ MODKEY,                       XK_KP_Left,      floatpos,             {.v = "-26a   0a" } }, // ←
	{ MODKEY,                       XK_KP_Right,     floatpos,             {.v = " 26a   0a" } }, // →
	{ MODKEY,                       XK_KP_End,       floatpos,             {.v = "-26a  26a" } }, // ↙
	{ MODKEY,                       XK_KP_Down,      floatpos,             {.v = "  0a  26a" } }, // ↓
	{ MODKEY,                       XK_KP_Page_Down, floatpos,             {.v = " 26a  26a" } }, // ↘
	{ MODKEY|ShiftMask,             XK_KP_Home,      floatpos,             {.v = "-26w -26h" } }, // ↖
	{ MODKEY|ShiftMask,             XK_KP_Up,        floatpos,             {.v = "  0w -26h" } }, // ↑
	{ MODKEY|ShiftMask,             XK_KP_Page_Up,   floatpos,             {.v = " 26w -26h" } }, // ↗
	{ MODKEY|ShiftMask,             XK_KP_Left,      floatpos,             {.v = "-26w   0h" } }, // ←
	{ MODKEY|ShiftMask,             XK_KP_Begin,     floatpos,             {.v = " 50% 50% 90%  80%" } }, // ·
	{ MODKEY|ShiftMask,             XK_KP_Right,     floatpos,             {.v = " 26w   0h" } }, // →
	{ MODKEY|ShiftMask,             XK_KP_End,       floatpos,             {.v = "-26w  26h" } }, // ↙
	{ MODKEY|ShiftMask,             XK_KP_Down,      floatpos,             {.v = "  0w  26h" } }, // ↓
	{ MODKEY|ShiftMask,             XK_KP_Page_Down, floatpos,             {.v = " 26w  26h" } }, // ↘
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1 } },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2 } },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3 } },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4 } },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5 } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

