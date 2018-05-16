@extends('layout.layout')

@section('content')
    <!-- VARE -->
    <div>
    @if (empty($varer))
        <h4>Handlekurven er tom.</h4>
    @endif

    <!-- VARER -->
    @foreach ($varer as $vare)
        <h4><img src="{{ Storage::disk('public')->url('thumb/'.$vare->varedata['bilde']) }}" style="max-height: 50px;"> {{ $vare->antall }} stk - {{ $vare->varedata['vare_navn'] }}</h4>
    @endforeach
    <hr>
    <h2>{{ $varer->sum('antall') }} varer i handlekurven. Pris totalt: <u>{{ number_format($pris,2,',','.') }}</u> kroner</h2>
    </div>
    <br>
    <button onclick="window.location.href='{{ url('/bestilt/1') }}'" class="btn btn-warning" style="float:right;">Send bestilling</button>

@stop