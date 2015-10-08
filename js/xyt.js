var viewer = OpenSeadragon({
        id: "chung-king-express",
        prefixUrl: "images/",
        tileSources: "dzi/ChungKing.dzi",
        homeFillsViewer: true,
        showNavigationControl: false
});

var vp = viewer.viewport;
var scroller;
var isRunning = false;

function panToStart() { vp.panTo(vp.imageToViewportCoordinates(0,540), true) };

function timePan() { vp.panBy(vp.imageToViewportCoordinates(1,0)) };
function startScroll() { scroller = setInterval(timePan, 41.70837504) };
function stopScroll() { clearInterval(scroller) };
function toggleScroll() {
    if ( isRunning == false ) {
        startScroll();
        $("#play-pause").text("PAUSE");
        isRunning = true;
    } else {
        stopScroll();
        $("#play-pause").text("PLAY");
        isRunning = false;
    }
};

// Initial position
setTimeout(panToStart, 50);
