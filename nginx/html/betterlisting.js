     //
            // Configure BetterListing here:
            var websiteName = 'Загрузки';
            var websiteURL = '*URL сайта*';
            // End of normal settings.
            //

            $(document).ready(function(){

            // Working on nginx HTML and applying settings.
            var text = $("h1").text();
            var array = text.split('/');
            var last = array[array.length-2];
            var dirStructure = $("a").text();
            var dirStructure = document.getElementsByTagName('a')[0].href;
            var dir = text.substring(10);
            var currentDir = last.charAt(0).toUpperCase() + last.slice(1);
            var dirTrun;

            // Truncate long folder names.
            if (currentDir.length > 19){
                var currentDir = currentDir.substring(0, 18) + '...';
            }

            // Updating page title.
            document.title = websiteName;

            // Add back button.
            $("h1").html('<p><img alt="логотип" style="margin-top: 7px; margin-left: 0px; padding: -1px; border-radius: 5px; border: 1px solid #aaa;" src="/.html/logo2.png"></p>');

            if (dir.length > 90) {
                dirTrun = dir.replace(/(.{90})/g, "$1\n")
            } else {
                dirTrun = dir.substring(0, dir.length - 1);
            }

            // Add subtitle and back arrow.
            $("h1").append('<h4><a href="' + dirStructure + '"></a><b>Current directory: </b>' + dirTrun.slice(0) + '</h4>');

            // Add search box.
            $("h1").prepend('<form id="custom-search-form" class="form-inline pull-right"><div class="btn-group"><input id="searchBox" placeholder="Найти на странице" type="search" class="form-control"> <span id="searchclear" class="glyphicon glyphicon-remove-circle"></span></div></form>');

            // Add parent directory bit.
            $("a").eq(1).html('Parent Directory');

            // Add titles.
            $("pre").prepend('<div class="header">Name                                                   Time                 Size</div>');

            // Establish supported formats.
            var list = new Array();
            var formats = ["bin", "jpg", "gif", "png", "html", "css", "zip", "iso", "tiff", "ico", "psd", "pdf", "exe", "rar", "deb", "swf", "7z", "doc", "docx", "xls", "xlsx", "pptx", "ppt", "txt", "php", "js", "c", "c++", "torrent", "sql", "wmv", "avi", "mp4", "mp3", "wma", "ogg", "msg", "wav", "py", "java", "gzip", "jpeg", "raw"];

            // Run when text is entered in the search box.
            $('#custom-search-form').on('input',function(e){
                e.preventDefault();
                var target = $('#searchBox').val();
                filter(target);
            });

            // Instant search.
            function filter(target){
                var parent_directory = 'parent directory';
                $('pre a').each(function(){
                    var arraySearch = $(this).attr('href');
                    var arraySearch = decodeURIComponent( arraySearch )
                    // Check the href data for searched term. Using href because the link label truncates if the file or folder name is too long.
                    // Special handling for 'Parent Directory' as the href data doesn't contain that word.
                    if (arraySearch.toLowerCase().indexOf(target.toLowerCase()) > -1 || (($(this).text() == 'Parent Directory') && (parent_directory.indexOf(target.toLowerCase()) > -1))){
                        $(this).show();
                        $($(this)[0].nextSibling).css('display', 'inline');
                    } else {
                        $(this).hide();
                        if($($(this)[0].nextSibling).hasClass('hideMe')) {
                            $($(this)[0].nextSibling).css('display', 'none');
                        } else {
                            $($(this)[0].nextSibling).wrap('<span class="hideMe" style="display:none"></style>');
                        }
                    }
                });
            }

            // Runs when clear button is hit.
            $("#searchclear").click(function(){
                $("#searchBox").val('');
                filter('');
            });

            // Scan all files in the directory, check the extensions and show the right MIME-type image.
            $('pre a').each(function(){
                var found = 0;
                var arraySplit = $(this).attr('href').split(".");
                var fileExt = arraySplit[arraySplit.length - 1];

                for (var i = 0; i < formats.length; i++) {
                    if (fileExt.toLowerCase() == formats[i].toLowerCase()) {
                        var found = 1;
                        var oldText = $(this).text();
                        $(this).html('<img class="icons" src="/.html/icons/' + formats[i] + '.png"></img></a>' + oldText);
                        return;
                    }
                }

                // Add an icon for the go-back link.
                if ($(this).text().indexOf("Parent Directory") >= 0) {
                    var found = 1;
                    var oldText = $(this).text();
                    $(this).html('<img class="icons" src="/.html/icons/home.png">' + oldText);
                    return;
                }

                // Check for folders as they don't have extensions.
                if ($(this).attr('href').substr($(this).attr('href').length - 1) == '/') {
                    var found = 1;
                    var oldText = $(this).text();
                    $(this).html('<img class="icons" src="/.html/icons/folder.png">' + oldText.substring(0, oldText.length - 1));

                    // Fix for annoying jQuery behaviour where inserted spaces are treated as new elements -- which breaks my search.
                    var string = ' ' + $($(this)[0].nextSibling).text();

                    // Copy the original meta-data string, append a space char and save it over the old string.
                    $($(this)[0].nextSibling).remove();
                    $(this).after(string);
                    return;
                }

                // File format not supported by Better Listings, so let's load a generic icon.
                if (found == 0){
                    var oldText = $(this).text();
                    $(this).html('<img class="icons" src="/.html/icons/error.png">' + oldText);
                    return;
                }
            });

            $(".icons").css("width", "4%");
            $(".icons").css("height", "4%");
        });
