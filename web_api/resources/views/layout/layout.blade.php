<!doctype html>
<html lang="{{ app()->getLocale() }}">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <title>Gruppe 10 - Kolonial.no</title>

        <!-- FONT -->
        <link href="https://fonts.googleapis.com/css?family=Raleway:100,600" rel="stylesheet" type="text/css">

        <!-- STYLE -->
        <link href="{{ asset('css/bootstrap-buttons.min.css') }}" rel="stylesheet" type="text/css">
        <style>
            html, body {
                background-color: #fff;
                color: #636b6f;
                font-family: 'Raleway', sans-serif;
                font-weight: 100;
                height: 100vh;
                margin: 0;
            }

            .full-height {
                height: 100vh;
            }

            .flex-center {
               /* align-items: center; */
                display: flex;
                justify-content: center;
            }

            .position-ref {
                position: relative;
            }

            .top-right {
                position: absolute;
                right: 10px;
                top: 18px;
            }

            .content {
                text-align: center;
            }

            .title {
                font-size: 84px;
            }

            .links > a {
                color: #636b6f;
                padding: 0 25px;
                font-size: 12px;
                font-weight: 600;
                letter-spacing: .1rem;
                text-decoration: none;
                text-transform: uppercase;
            }

            .links > a:hover {
                text-decoration: underline;
                text-decoration-color: orange;
            }

            .m-b-md {
                margin-bottom: 30px;
            }

            .form-control:focus {
                border-color: orange;
            }

            .justify-content-center {
              -ms-flex-pack: center !important;
                justify-content: center !important;
            }

            ul.pagination li {
                display: inline;
                font-size: 12px;
                font-weight: bold;
            }

            ul.pagination li a {

                color: black;
                padding: 8px 8px;
                text-decoration: none;
                transition: background-color .3s;
                border: 1px orange;
                margin: 4px;
            }

            ul.pagination li a.active {
                background-color: orange;
                padding: 8px 8px;
                margin: 4px;
                color: white;
                border: 1px solid black;
            }

            ul.pagination li.active {
                /*background-color: #4CAF50;*/
                background-color: orange;
                padding: 8px 8px;
                margin: 4px;
                color: white;
                border: 1px solid black;
            }

            /*ul.pagination li a:hover:not(.active) {background-color: #ddd;}*/
            ul.pagination li a:hover {background-color: #FFC04C;}

            ul.pagination li.disabled {
                /*background-color: #cccccc;*/
                color: #ddd;
                padding: 8px 8px;
                border: 1px solid #ddd;
                margin: 4px;
            }
        </style>
        
        @yield('header')
    </head>
    <body>
        <div class="flex-center position-ref full-height">
            @if (Route::has('login'))
                <div class="top-right links">
                    @auth
                        <a href="{{ url('/home') }}">Home</a>
                    @else
                        <a href="{{ route('login') }}">Login</a>
                        <a href="{{ route('register') }}">Register</a>
                    @endauth
                </div>
            @endif

            <div class="content">
                <a href="{{ url('/') }}">
                    <img src="{{ asset('/assets/1599.png') }}" height="200px">
                </a>
                <div class="links">
                    <a href="{{ url('/vare/ny') }}">Registrer varer</a>
                    <a href="{{ url('/varer') }}">Vis alle varer</a>
                    <a href="{{ url('/handlekurv') }}">Vis handlekurv</a>
                </div>
                <hr>

            @yield('content')
            </div>

        </div>
    </body>
</html>
