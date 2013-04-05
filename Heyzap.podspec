Pod::Spec.new do |s|
  s.name         = "Heyzap"
  s.version      = "3.4.34"
  s.summary      = "Heyzap's iOS SDK: Leaderboards, Achievements, and Ads for games."
  s.description  = <<-DESC
                        Random tex t that is really long and will be longer than the summary testing if this works. Does it work? We'll see.
                    DESC
  s.homepage     = "http://developers.heyzap.com/docs"
  # s.screenshots  = "www.example.com/screenshots_1", "www.example.com/screenshots_2"

  s.license = { :type => 'Commercial', :text => 'See http://developers.heyzap.com/docs' } # Need a real license/terms site.
  s.author       = { "Heyzap" => "integration@heyzap.com" }

  s.source       = { :git => "https://github.com/MaxGabriel/HeyzapSDK.git", :tag => "3.4.34" }
  s.platform     = :ios, '4.3'
  s.resources = "Heyzap.bundle/*.png"

  s.source_files = 'HeyzapDummy.{m,h}'

  s.preserve_paths = 'Heyzap.framework'

  s.frameworks = 'QuartzCore', 'CoreGraphics', 'Heyzap'
  s.weak_frameworks = 'StoreKit','AdSupport'

  s.requires_arc = true
  s.xcconfig = { 'FRAMEWORK_SEARCH_PATHS' => '"$(PODS_ROOT)/Heyzap"' }
end
