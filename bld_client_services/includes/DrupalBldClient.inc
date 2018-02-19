<?php

class DrupalBldClient {

  private $protocols;


  /**
   * ConfigServicesForm constructor.
   */
  public function __construct() {
    $this->protocols = [
      'http' => t('HTTP'),
      'https' => t('HTTPS'),
    ];
  }

  /**
   * @return array
   */
  public function getProtocols() {
    return $this->protocols;
  }

  public function getSourceService($companyCode, $storeCode, $page, $size = 10) {
    $protocol = variable_get('bld.services_client.protocol','');
    $endPoint = variable_get('bld.services_client.url','');
    $key = variable_get('bld.services_client.key','');
    $path = "$protocol://$endPoint/$companyCode/$storeCode/$key?page=$page&size=$size";
    //$path = "$protocol://$endPoint/$companyCode/All/$key?page=$page&size=$size";
   // var_dump($path);
   // $path = "https://api.ark09.net/NewsService.svc/003/822/GetNews?page=1&size=100";

    // $path = "https://api.ark09.net/NewsService.svc/003/822/GetNews?page=1&size=100";
    // The result order is not guaranteed, so check each term separately.
    $url = url($path, ['absolute' => TRUE]);
    $result = drupal_http_request($url);
    if (isset($result->data)) {
      $data = drupal_json_decode($result->data);
     // var_dump($data['GetNewsResult'][0]['PublishDate']);die;
      return $data;
    }
    else {
      return [];
    }
  }

}
