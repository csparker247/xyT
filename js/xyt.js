var viewer = OpenSeadragon({
        id: "chung-king-express",
        prefixUrl: "images/",
        tileSources: "dzi/ChungKing.dzi",
        homeFillsViewer: true,
        showNavigationControl: false
});

var vp = viewer.viewport;
var image;
var thumb = $("#thumb").get(0);

viewer.addHandler('pan', function (e) {
  if (thumb.paused) {
    var frame = vp.getCenter().x * viewer.world.getItemAt(0).getContentSize().x;
    var second = frame / (24000/1001);
    thumb.currentTime = second;
  }
});

viewer.addHandler('canvas-drag', function(e) {
  if (isRunning)
    thumb.pause();
});

viewer.addHandler('canvas-drag-end', function(e) {
  if (isRunning)
    thumb.play();
});

var scroller;
var isRunning = false;

function panToStart() { vp.panTo(vp.imageToViewportCoordinates(0,540), true) };

function timePan() {
  if( !thumb.paused) {
    var frame = thumb.currentTime * (24000/1001);
    var pos = vp.imageToViewportCoordinates(frame,540);
    vp.panTo(pos);
  }
};

function startScroll() {
  thumb.play();
  scroller = setInterval(timePan, 1)
};

function stopScroll() { thumb.pause(); clearInterval(scroller) };
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

// On-Load Setup
$(window).load(function() {
  panToStart();
});
