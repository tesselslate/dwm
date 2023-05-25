/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrains Mono:size=12", "JetBrainsMono Nerd Font Mono:size=20" };

static char termcol0[] = "#000000"; /* black   */
static char termcol1[] = "#ff0000"; /* red     */
static char termcol2[] = "#33ff00"; /* green   */
static char termcol3[] = "#ff0099"; /* yellow  */
static char termcol4[] = "#0066ff"; /* blue    */
static char termcol5[] = "#cc00ff"; /* magenta */
static char termcol6[] = "#00ffff"; /* cyan    */
static char termcol7[] = "#d0d0d0"; /* white   */
static char termcol8[]  = "#808080"; /* black   */
static char termcol9[]  = "#ff0000"; /* red     */
static char termcol10[] = "#33ff00"; /* green   */
static char termcol11[] = "#ff0099"; /* yellow  */
static char termcol12[] = "#0066ff"; /* blue    */
static char termcol13[] = "#cc00ff"; /* magenta */
static char termcol14[] = "#00ffff"; /* cyan    */
static char termcol15[] = "#ffffff"; /* white   */
static char *termcolor[] = {
  termcol0,
  termcol1,
  termcol2,
  termcol3,
  termcol4,
  termcol5,
  termcol6,
  termcol7,
  termcol8,
  termcol9,
  termcol10,
  termcol11,
  termcol12,
  termcol13,
  termcol14,
  termcol15,
};

static char normbgcolor[] = "#24283b";
static char normbordercolor[] = "#292e42";
static char normfgcolor[] = "#c0caf5";
static char selbgcolor[] = "#292e42";
static char selbordercolor[] = "#89ddff";
static char selfgcolor[] = "#c0caf5";

static const char *colors[][3]      = {
	/*               fg           bg           border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor },
};

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "⛶", "⛶", "⛶", "⛶" };
static const char *tagsel[][2] = {
	{ termcol1, "#000000" },
	{ termcol3, "#000000" },
	{ termcol6, "#000000" },
	{ termcol2, "#000000" },
};

static const unsigned int tagalpha[] = { OPAQUE, baralpha };
static const unsigned int ulinepad	= 6;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance        title       tags    isfloating  monitor */
    { "Minecraft",  "Minecraft",    NULL,       0,      1,          -1 },
    { NULL,         NULL,           "Ninjabr",  0,      1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/fish", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "kitty", NULL };

/* custom funcs */
static void fullscreen(const Arg *arg);

/* keys */
#include "movestack.c"
#include "unfloat.c"
static Key keys[] = {
    /* modifier                     key         function        argument */

    // terminal
    { MODKEY|ShiftMask,             XK_Return,                  spawn,          {.v = termcmd } },

    // menu
	{ MODKEY,                       XK_Return,                  spawn,          SHCMD("menu") },

    // lock
    { MODKEY|ShiftMask,             XK_l,                       spawn,          SHCMD("lock") },

    // fullscreen screenshot
    { MODKEY,                       XK_Print,                   spawn,          SHCMD("screenshot-clipboard -g 1920x1080+0+0") },

    // select screenshot
    { MODKEY|ShiftMask,             XK_Print,                   spawn,          SHCMD("screenshot-clipboard -s") },

    // lower volume
    { 0,                            XF86XK_AudioLowerVolume,    spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },

    // raise volume
    { 0,                            XF86XK_AudioRaiseVolume,    spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },

    // pause/play mpd
    { 0,                            XF86XK_AudioPlay,           spawn,          SHCMD("mpc toggle") },

    // next track in mpd
    { 0,                            XF86XK_AudioNext,           spawn,          SHCMD("mpc next") },

    // prev track in mpd
    { 0,                            XF86XK_AudioPrev,           spawn,          SHCMD("mpc prev") },

    // focus next window
	{ MODKEY,                       XK_j,                       focusstack,     {.i = +1 } },

    // focus prev window
	{ MODKEY,                       XK_k,                       focusstack,     {.i = -1 } },

    // move window up stack
	{ MODKEY|ShiftMask,             XK_j,                       movestack,      {.i = -1 } },

    // move window down stack
	{ MODKEY|ShiftMask,             XK_k,                       movestack,      {.i = +1 } },

    // increase master size
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },

    // decrease master size
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },

    // view next tag
	{ MODKEY|ControlMask,           XK_Right,                   viewnext,       {0} },

    // view next tag
    { MODKEY|ControlMask,           XK_l,                       viewnext,       {0} },

    // view previous tag
	{ MODKEY|ControlMask,           XK_Left,                    viewprev,       {0} },

    // view previous tag
	{ MODKEY|ControlMask,           XK_h,                       viewprev,       {0} },

    // next monitor
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },

    // prev monitor
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },

    // move window to next tag
	{ MODKEY|ShiftMask,             XK_Right,                   tagtonext,      {0} },

    // move window to prev tag
	{ MODKEY|ShiftMask,             XK_Left,                    tagtoprev,      {0} },

    // move window to next monitor
	{ MODKEY|ShiftMask,             XK_comma,                   tagnextmon,      {0} },

    // move window to prev monitor
	{ MODKEY|ShiftMask,             XK_period,                  tagprevmon,      {0} },

    // close window
	{ MODKEY,                       XK_c,                       killclient,     {0} },

    // fullscreen window
    { MODKEY,                       XK_f,                       fullscreen,     {0} },

    // unfloat visible
    { MODKEY|ShiftMask,             XK_f,                       unfloatvisible, {0} },

    // quit dwm
	{ MODKEY|ShiftMask,             XK_q,                       quit,           {0} },

    // reload xresources
	{ MODKEY,                       XK_F5,                      xrdb,           {.v = NULL } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
};

/* function impls */
void
fullscreen(const Arg *arg) {
    Client *c = selmon->sel;
    if (c != NULL)
        setfullscreen(c, !c->isfullscreen);
}
