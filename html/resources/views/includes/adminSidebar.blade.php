<nav class="navbar-default navbar-static-side" role="navigation">
    <div class="sidebar-collapse">
        <ul class="nav metismenu" id="side-menu">
            <li class="nav-header">
                <div class="dropdown profile-element"> <span>

                            <img alt="image" class="img-circle" width="70" height="70" src="/img/admin/userImg.jpg"/>

                             </span>
                    <a data-toggle="dropdown" class="dropdown-toggle" href="#">
                            <span class="clear">
                                <span class="block m-t-xs"> <strong class="font-bold">{{ Auth::user()->fName.' '.Auth::user()->lName}}</strong>
                             </span>
                                 </span>
                    </a>
                </div>
                <div class="logo-element">
                    IN+
                </div>
            </li>


            <li>
                <a href="/"><i class="fa fa-laptop"></i> <span class="nav-label">تغییر رمز عبور</span></a>
            </li>
            <li>
                <a href="/network-setting"><i class="fa fa-table"></i> <span class="nav-label">تنظیمات شبکه</span></a>
            <li>
                <a href="/server-setting"><i class="fa fa-table"></i> <span class="nav-label">تنظیمات برنامه</span></a>
            </li>



        </ul>

    </div>
</nav>