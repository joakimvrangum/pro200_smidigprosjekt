@extends('layout.layout')

@section('content')
    <!-- VARE -->
    <div>
    @foreach ($varer as $vare)
        <h4>{{ $vare->vare_navn }}</h4>
    @endforeach

    <hr>
    <h2>{{ $varer->total() }} varer tilgjengelig i kolonialen.</h2>
    </div>

    <!-- SIDER (20 varer) -->
    <div id="images-pagination-wrapper">
        {{ $varer->links() }}
    </div>
@stop